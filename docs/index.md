# 9lang

9lang is an esoteric programming language. A 9lang program ends with the `.9` extension. A 9lang program is made of 3x3 instruction grid. Each caracter can be either a space or any other caracter. The first instruction to be executed is the one at `0:0`. When the program starts,
the [direction](direction.md) is right. The [interpreter](interpreter.md) is named `9`. You can call it just using the `9 <program path>` command (if the command path contains spaces put `"` around it).

## Examples

> ```
> o oo oo o   o o
>     o     o  o
>    o o      o o
> ```
>
> This program just prints an `x` character to *stdout*.
>
> *Note:* here the `o` character is used. We migth use any other character instead but *space*.
>
> #### Explanation
>
> *(`.`s represent spaces and `X`s represent any other character)*
>
>> - ```
>>   X.X
>>   ...
>>   ...
>>   ```
>>   enable the [*ascii mode*](ascii_mode.md).
>
>> - ```
>>   X.X
>>   .X.
>>   X.X
>>   ```
>>   (In [*ascii mode*](ascii_mode.md)) [stack](stack.md) the 'x' character.
>
>> - ```
>>   X.X
>>   ...
>>   ...
>>   ```
>>   (In [*ascii mode*](ascii_mode.md)) disable the [*ascii mode*](ascii_mode.md).
>
>> - ```
>>   ...
>>   .X.
>>   ...
>>   ```
>>   [unstack](stack.md) a character and print it to *stdout*.
>
>> - ```
>>   X.X
>>   .X.
>>   X.X
>>   ```
>>   exit the program.
>>
>
