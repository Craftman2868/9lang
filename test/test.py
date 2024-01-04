import json, subprocess, os
from sys import stderr, argv
import datetime

CONFIG_PATH = "config.json"
DEFAULT_TEST_DIR = "tests"
DEFAULT_COMMAND = "{bin} {program}"

step = None
config = None

ESC =       "\x1B"
CSI =       ESC + "["
C_RESET =   CSI + "0m"
C_RED =     CSI + "31m"
C_GREEN =   CSI + "32m"
C_YELLOW =  CSI + "33m"

logFile = None

testResults = {}

def genLogName():
    return datetime.datetime.now().strftime("%y-%m-%d %H:%M:%S.log")

def log(*txt, err=False):
    global logFile

    if not logFile:
        if config and "outputDir" in config:
            if not os.path.exists(config["outputDir"]):
                try:
                    os.mkdir(config["outputDir"])
                except OSError:
                    error(f"output directory ({config['outputDir']}) does not exist and cannot be created")
            if not os.path.isdir(config["outputDir"]):
                error(f"output directory ({config['outputDir']}) is not a directory")
            name = os.path.join(config["outputDir"], genLogName())
            logFile = open(name, "w")
            printLog(f"Logging to {name}")

    if logFile:
        print(" ".join(str(a).replace(C_RESET, "").replace(C_RED, "").replace(C_GREEN, "").replace(C_YELLOW, "") for a in txt), file=logFile)
    print(*txt, file=(stderr if err else None))

def closeLog():
    if logFile:
        logFile.close()

def printError(msg: str = "Error"):
    log(f"{C_RED}[{step.center(8)}]{C_RESET} {' ' if msg.startswith(' ') else '-'} {msg}", err=True)

def error(msg: str = None):
    printError(msg)
    closeLog()
    exit()

def printSuccess(msg: str = None):
    log(f"{C_GREEN}[{step.center(8)}]{C_RESET}{(('   ' if msg.startswith(' ') else ' - ') + msg) if msg else ''}")

def printWarning(msg: str = None):
    log(f"{C_YELLOW}[{step.center(8)}]{C_RESET}{(('   ' if msg.startswith(' ') else ' - ') + msg) if msg else ''}")

def printCommand(command: str):
    log(f"[{step.center(8)}] > {command}")

def printLog(msg: str = None):
    log(f"[{step.center(8)}]{(' - ' + msg) if msg else ''}")

def test(test, err: str, suc: str = None):
    if not test:
        error(err)

    if suc:
        printSuccess(suc)

    return test

def runCommand(c):
    printCommand(c)

    res = os.system(c)

    test(res == 0, "Exit code != 0", "Command ran successfully")

    return res

def stepDone():
    if step:
        printSuccess("Done")

def changeStep(s):
    global step

    stepDone()

    step = s

    printLog()

def loadConfig() -> dict:
    with open(CONFIG_PATH, "r") as f:
        config = json.load(f)

    return config

def testSteps():
    l = os.listdir(config.get("testDir", DEFAULT_TEST_DIR))
    l.sort()

    for f in l:
        if os.path.isdir(os.path.join(config.get("testDir", DEFAULT_TEST_DIR), f)):
            yield f

def tests(testStep):
    l = os.listdir(os.path.join(config.get("testDir", DEFAULT_TEST_DIR), testStep))
    l.sort()

    for f in l:
        if os.path.isfile(os.path.join(config.get("testDir", DEFAULT_TEST_DIR), testStep, f)) and f.endswith(".test"):
            yield f

def testCommand(command, testConfig):
    res = {}

    try:
        completed_proc = subprocess.run(args=command, shell=True, text=True, capture_output=True, timeout=testConfig.get("timeout", None), input=testConfig.get("in", ""))
    except subprocess.TimeoutExpired as e:
        res["timeout"] = False
        res["stderr"] = e.stderr
        res["stdout"] = e.stdout
        res["returncode"] = None
        if "code" in testConfig:
            res["code"] = Ellipsis
    else:
        if "timeout" in testConfig:
            res["timeout"] = True

        res["stderr"] = completed_proc.stderr
        res["stdout"] = completed_proc.stdout
        res["returncode"] = completed_proc.returncode

        if "code" in testConfig:
            res["code"] = completed_proc.returncode == testConfig["code"]

        if "out" in testConfig:
            res["out"] = completed_proc.stdout == testConfig["out"]

    testExpect = testConfig.get("expect", "success")

    if testExpect == "success":
        for t, success in res.items():
            if isinstance(success, bool) and not success:
                res["test"] = False
                break
        else:
            res["test"] = True
    elif testExpect == "timeout":
        assert "timeout" in testConfig

        res["test"] = not res["timeout"]
    else:
        error(f"Unknown expectation '{testExpect}'")

    return res

def runTest(testStep, t):
    testDir = os.path.join(config.get("testDir", DEFAULT_TEST_DIR), testStep)
    testFilePath = os.path.join(testDir, t)
    testRes = testResults[testStep][t]

    try:
        with open(testFilePath, "r") as f:
            testConfig = json.load(f)
    except:
        printError(f"cannot load {testFilePath} (for test {testStep}/{t})")
        closeLog()
        raise

    command = config.get("command", DEFAULT_COMMAND).format(**{
        "bin": config["bin"],
        "program": os.path.join(testDir, testConfig["program"]),
    })

    testRes.update(testCommand(command, testConfig))

    testRes["expect"] = testConfig

    return testRes

MAX_LENGTH = 1500

def toString(s):
    if isinstance(s, bytes):
        s = s.decode()
    else:
        s = str(s)

    if len(s) > MAX_LENGTH:
        s = s[:MAX_LENGTH] + f"[+{len(s) - MAX_LENGTH} characters]"

    return s

def printTestResCond(name, success):
    if success == True:
        return C_GREEN + name
    elif success == False:
        return C_RED + name
    else:
        return C_YELLOW + name

def printTestResConds(res):
    return f"{C_RESET}, ".join(printTestResCond(name, success) for name, success in res.items() if name != "test" and (isinstance(success, bool) or success == Ellipsis)) + C_RESET

def printTestRes(testStep, t, res):
    if res["test"]:  # success
        printSuccess(f"{testStep}/{t} - {C_GREEN}Success: " + printTestResConds(res))
    else:
        printError(f"{testStep}/{t} - {C_RED}Error: " + printTestResConds(res))
        printError(f"{testStep}/{t} stderr:\n" + toString(res["stderr"]))
        if "out" in res and not res["out"]:
            printError(f"{testStep}/{t} stdout:\n" + toString(res["stdout"]))
            printError(f"{testStep}/{t} expected stdout:\n" + str(res["expect"]["out"]))
        if "code" in res and not res["code"]:
            printError(f"{testStep}/{t} return code: {res['returncode']}")
            printError(f"{testStep}/{t} expected code: {res['expect']['code']}")
    if "timeout" in res and not res["timeout"]:
        p = printSuccess if res["test"] else printError

        p(f"{testStep}/{t} time exceeded (max time: {res['expect']['timeout']})")

def main():
    global config

    askedTests = None

    if len(argv) > 1:
        askedTests = argv[1:]

    changeStep("Init")

    printLog("Loading config...")
    try:
        config = loadConfig()

        assert "bin" in config
    except:
        printError("Error loading config")
        closeLog()
        raise
    else:
        printSuccess("Config loaded")

    if "initCommand" in config:
        runCommand(config["initCommand"])

    changeStep("Test")

    ts = testSteps()

    for testStep in ts:
        if askedTests:
            if testStep not in askedTests:
                continue

        testResults[testStep] = {
            "_success": True,
            "_failure": True,
        }
        printLog(f"Testing {testStep}")
        for t in tests(testStep):
            testResults[testStep][t] = {}
            printLog(f"Testing {testStep}/{t}")
            res = runTest(testStep, t)

            printTestRes(testStep, t, res)

            if not res["test"]:
                testResults[testStep]["_success"] = False
            else:
                testResults[testStep]["_failure"] = False

            printSuccess(f"{testStep}/{t} done!")

        if testResults[testStep]["_success"]:
            testResults[testStep]["_failure"] = False
        
        printSuccess(f"{testStep} tested!")

    changeStep("Result")

    passed = sum([res["_success"] for res in testResults.values()])
    failed = sum([res["_failure"] for res in testResults.values()])
    other  = len(testResults) - (passed + failed)

    if passed or other:
        printSuccess("Passed tests:")
        for testStep, res in testResults.items():
            if res["_success"]:
                printSuccess(f" -> {testStep}")
            elif res["_failure"]:
                continue
            else:
                printWarning(f" -> {testStep}")
            for t, r in res.items():
                if not isinstance(r, dict):
                    continue
                if r["test"]:
                    printSuccess(f"    => {t}: " + printTestResConds(r))

    if failed or other:
        printError("Failed tests:")
        for testStep, res in testResults.items():
            if res["_success"]:
                continue
            elif res["_failure"]:
                printError(f" -> {testStep}")
            else:
                printWarning(f" -> {testStep}")
            for t, r in res.items():
                if not isinstance(r, dict):
                    continue
                if not r["test"]:
                    printError(f"    => {t}: " + printTestResConds(r))

    printLog(f"[{C_GREEN}{'='*passed}{C_YELLOW}{'~'*other}{C_RESET}{' '*failed}]")

    closeLog()


if __name__ == "__main__":
    main()
