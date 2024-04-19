# Chip 8 opcoes
https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set

## Functions
Opcodes `Ret 00EE`,`Call 2NNN`

```chip8


NEEDS XO-Chip Extension
=====Function def=====

# if return with value load value in to register
00EE # Exit function
======================

======Function Call===

# load register with function params
FX65
6XNN 

2NNN # move to function location

======================

```


## Loops 

### While
Opcodes `1NNN`, expr


### Loop 
Opcodes `1NNN`

### for in
Opcodes `1NNN`, expr

## Expr

## Variables


## External Functions 

`clear_screen(): void`
Opcode `00E0`
`write(x: u8, y: u8, height: u8): void`
Opcode `DXYN`
`rand(range: u8): u8;`
Opcode `CXNN`
`key_pressed(key: u8): bool`
Opcode `EX9E`
`key_unpressed(key: u8): bool`
Opcode `EXA1`
`wait_for_key(key: u8): u8`
Opcode `FX0A`
`delay(u8): void`
Opcodes `FX07`,`1NNN`,`FX15`
