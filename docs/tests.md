# Tests

There is a python test script in the `test` directory. You can execute it using the following commands:
```sh
cd test
python test.py
```

The `test/tests` directory containts one directory per tested feature, in each of them there is at least one `.test` and at least one `.9` program.


## `.test` format

The `.test` files must be valid JSON files.

```json
{
    // Required
    "program": "<program name>",

    // Optional
    "out": "<expected stdout>",
    "in": "<input sent to the program's stdin>",
    "timeout": "<maximum time>",
    "expect": "<expected result>",
    "code": <expected return code>
}
```

|Key    |Required|Type  |Value                                              |
|-------|:------:|------|---------------------------------------------------|
|`program`|   x    |string|The program name (in the current test directory)   |
|`out`    |        |string|The expected *stdout*                                |
|`in`     |        |string|The text that will be sent to the program's *stdin*  |
|`timeout`|        |number|The maximum time that the program have (if exceeded, the program is stopped)|
|`expect` |        |number|The expected result (can be either `success` (default) or `timeout`)|
|`code`   |        |number|The expected return code                           |


## Examples

Look at the `test/tests` directory for examples.
