
use std::{fmt::{ Display }, path::PathBuf};
#[derive(Default,Debug,Clone)]
pub struct Position {
    pub row: usize,
    pub col: usize
}
impl Position {
    pub fn new(row: usize, col: usize) -> Self {
        Self {
            row,
            col
        }
    }
    pub fn dec_col(&mut self) {
        self.col -= 1;
    }
    pub fn inc_col(&mut self) {
        self.col += 1;
    }
    pub fn inc_row(&mut self) {
        self.row += 1;
    }
    pub fn set_row(&mut self, row: usize) {
        self.row = row;
    }
    pub fn set_col(&mut self, col: usize) {
        self.col = col;
    }
}

impl Display for Position {
    fn fmt(&self, formatter: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(formatter,"{}:{}",self.col,self.row)
    }
}

#[derive(Debug, Clone,PartialEq)]
pub enum TokenType {
    INTEGER(isize),
    STRING(String),
    BOOL(bool),
    KEYWORD(String),
    IDENTIFIER(String),
    DIV,
    MUL,
    PLUS,
    MINUS,
    LPAREN, // (
    RPAREN, // )
    LBRACKET, // ],
    RBRACKET, // [,
    LCURLY, // {
    RCURLY, // }
    SEMICOLON, // ;
    NEWLINE, // \n
    EQUAL,
    COLON,
    COMMA,
    NOT,
    ARROWL,
    ARROWR,
    GEATERTHENEQ,
    LESSTHENEQ,
    EE,
    DOT,
    AND,
    OR,
    EOF
}

#[derive(Debug,Clone)]
pub struct Token {
    pub start: Position,
    pub end: Position,
    pub token: TokenType,
}

impl Token {
    pub fn new(token: TokenType, start: Position, end: Position) -> Self {
        Self {
            token,
            start,
            end
        }
    }
    pub fn empty() -> Self {
        Self {
            token: TokenType::EOF,
            start: Position::default(),
            end: Position::default()
        }
    }
    pub fn is(&self, token: TokenType) -> bool {
        std::mem::discriminant(&self.token) == std::mem::discriminant(&token)
    }
    pub fn is_keyword<T>(&self, keyword: T) -> bool where T: ToString {
        self.token == TokenType::KEYWORD(keyword.to_string())
    }
}


#[derive(Clone, Debug)]
pub enum Node {
    Empty,
    Integer {
        start: Position,
        end: Position,
        value: TokenType
    },
    String {
        start: Position,
        end: Position,
        value: TokenType
    },
    VarableAccess {
        start: Position,
        end: Position,
        identifer: TokenType
    },
    VarableAsignment {
        identifer: Token,
        operation: Token,
        value: Box<Node>,
        start: Position,
        end: Position
    },
    VarableDeclarement {
        identifer: Token,
        is_const: bool,
        value: Box<Node>,
        start: Position,
        end: Position
    },
    FunctionCall {
        identifer: Box<Node>,
        arguments: Vec<Node>,
        start: Position,
        end: Position
    },
    UnaryOperation {
        operator: Token,
        value: Box<Node>,
        start: Position,
        end: Position
    },
    BinOperation {
        left: Box<Node>,
        operator: TokenType,
        right: Box<Node>,
        start: Position,
        end: Position
    },
    Scope {
        statements: Vec<Box<Node>>
    },
    Object {
        props: Vec<(Token,Node)>,
        start: Position,
        end: Position
    },
    List {
        values: Vec<Node>,
        start: Position,
        end: Position
    },
    IndexingAccess {
        identifier: Token,
        index: Box<Node>,
        start: Position,
        end: Position
    },
    If {
        start: Position,
        end: Position,
        expr: Box<Node>,
        body: Box<Node>
    },
    Function {
        start: Position,
        end: Position,
        args: Vec<TokenType>,
        body: Box<Node>,
        name: String
    },
    Return {
        start: Position,
        end: Position,
        node: Box<Node>
    },
    For {
        start: Position,
        end: Position,
        iterator: Box<Node>,
        element_key: String,
        scope: Box<Node>
    },
    While {
        start: Position,
        end: Position,
        scope: Box<Node>,
        expr: Box<Node>
    },
    Namespace {
        start: Position,
        end: Position,
        identifer: String,
        scope: Box<Node>
    },
    Import {
        start: Position,
        end: Position,
        from: PathBuf,
        namespace: String
    },
    PropAccess {
        start: Position,
        end: Position,
        path: Vec<String>
    }
}


#[cfg(test)] 
mod tests {
    use super::*;
    fn init() {
        simple_logger::SimpleLogger::new().init().unwrap();
    }
    #[test]
    fn test_token_is_keyword() {
        init();

        let test = Token::new(TokenType::KEYWORD("import".to_string()),Position::default(),Position::default());

        assert!(test.is_keyword("import"));

        assert!(!test.is_keyword("return"));
    }
    #[test]
    fn test_token_is(){
       // init();

        let test_b_value = Token::new(TokenType::INTEGER(3),Position::default(),Position::default());
        let test_w_value = Token::new(TokenType::KEYWORD("import".to_string()),Position::default(),Position::default());
        let test_n_value = Token::new(TokenType::EOF,Position::default(),Position::default());

        assert!(test_w_value.is(TokenType::KEYWORD("import".to_string())));

        assert!(test_n_value.is(TokenType::EOF));

        assert!(test_b_value.is(TokenType::INTEGER(3)));
        assert!(test_b_value.is(TokenType::INTEGER(4)));
    }
}