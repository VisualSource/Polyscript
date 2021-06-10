#[derive(Debug, Clone, Copy, PartialEq)]
pub enum TokenType {
    DIV, // \
    MUL, // *
    PLUS,
    MINUS, // -
    POWER, // ^
    INT, // whole number
    FLOAT, // floating point number
    STRING, // "STRING"
    LPAREN,// (
    RPAREN, // )
    LBRACKET, // [
    RBRACKET, // ]
    LCURLYBRACKET, // {
    RCURLYBRACKET, // }
    IDENTIFIER, // a list of chars
    KEYWORD, // defined keywords
    COLON, // : 
    SEMICOLON, // ;
    PATHSEPERATOR, // :: 
    NOT, // !,
    AND, // &&,
    OR, // ||,
    EQ, // =
    COMMA, // ,
    RETURNARROW,// -> Function return type, Closure return type, Function pointer type
    FATARROW, // =>  Match arms, Macros 
    EE, // ==
    NE, // !=
    LT, // <
    GT, // >
    LTE, // <=
    GTE, // >= 
    EOF,
    NEWLINE, // \n
    PLUSEQ, // +=
    MINUSEQ, // -=
    DIVEQ, // \=
    MULEQ, // *=
    PLUSPLUS,// ++
    MINUSMINUS, // --
    DOT, // .
}
impl TokenType {
    pub fn to_string(&self) -> String {
        format!("{:?}",self).to_string()
    }
}