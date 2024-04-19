
# Vip Spec

[Variables](#variable)
    1. [let](#let)
    1. [const](#const)
[If](#if)
[Function](#function)
    - [Named Functions](#named-functions)
    - [Lamadas](#lamadas)
    - [Return](#return)

Implemeted
- [x] let
- [ ] const
- [x] if
- [x] fn
- [ ] lamada
- [x] return
- [ ] struct
- [ ] impl 
- [ ] //
- [ ] /**/
- [ ] from 
- [ ] import 
- [ ] export
- [ ] format string
- [ ] types unsigned ints
- [ ] types signed ints
- [ ] type
- [ ] str type
- [ ] arary
- [X] while
- [ ] for
- [ ] loop
- [ ] extern

## Variable

### let
Let variable
`let a = 4;`

#### const
`const a = 4;`

## If 

Keyword: if

`if expr {}`

## Function

### Named Functions

Keyword: fn

`fn test(){}`
`fn test(arg: u8): void {}`

### Lamadas

`() => {}`
`(): void => {}`

### Return 

Keyword: return

`return`

## Struct 

Keyword: struct

```
struct App {
    world: str,
    hello: str
}

impl App {
    fn say(self): str {
        return self.hello + self.world;
    }
}
```

## Comment

### Single

`// This is a single line comment`
### Multi

`/* This is a multi line comment */`

## Type

### type

Keyword: type

`type A = u8`

### Int

#### unsigned int
- u8
- u16
- u32
- u64
- usize

#### signed int

- i8
- i16
- i32
- i64
- isize

`let a: u8 = 1;`

### Void 

- void

### String

- str

Example 
`let value: str = "Hello, Woirld";`

#### Format String

`let value = f"{variable_name}"; `


### Array

Max array 
`let value: array[] = [];` 

## Modules

### Import 

Keywords: from, import

```
import module_name;


module_name::somefunciton()
```

```
from test import somefunciton;

somefunction();
```
### Export

Keywords: export

`export fn somefunction(): void {}`

## Loops

### while 

Keywords: while

`while expr {}`

### for

Keywords: for, in

`for var in expr {}`

### loop

Keyword: loop

`loop {}`

## Extern

Keyword: extern

`extern println(value: any);`