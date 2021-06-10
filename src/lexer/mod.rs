use crate::shared::{ postion::Postion, token::Token, token_type::TokenType };
use crate::errors::{ IllegalCharError, ExpectedCharError, error::Error };
pub struct Lexer {
    text: String,
    file_name: String,
    pos: Postion,
    current_char: char
}
impl Lexer {
    pub fn new(text: String, file_name: String) -> Self {
        let mut a = Self {
            text,
            file_name,
            pos: Postion::default(),
            current_char: '\0'
        };
        a.advance();
        a
    }
    pub fn make_tokens(&mut self) -> Result<(Vec<Token>, String), Box<dyn Error>> {
        let mut tokens = vec![];

        self.current_char.is_alphabetic();
        while self.current_char != '\0' {
            match self.current_char {
                a if a.is_whitespace() => self.advance(),
                a if a.is_ascii_digit() => tokens.push(self.make_number()),
                '\n' => {
                    tokens.push(Token::new(TokenType::NEWLINE, None, self.pos.clone(), None));
                    self.advance();
                },
                a if a.is_control() => self.advance(),
                
                a if a.is_alphabetic() || a == '_' => {
                    tokens.push(self.make_identifer());
                }
                ':' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::COLON;
                    self.advance();
                    if self.current_char == ':' {
                        token = TokenType::PATHSEPERATOR;
                        self.advance();
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '^' => {
                    tokens.push(Token::new(TokenType::POWER, None, self.pos.clone(), None));
                    self.advance();
                },
                '+' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::PLUS;
                    self.advance();

                    match self.current_char {
                        '+' => {
                            token = TokenType::PLUSPLUS;
                            self.advance();
                        }
                        '=' => {
                            token = TokenType::PLUSEQ;
                            self.advance();
                        }
                        _ => {}
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '-' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::MINUS;
                    self.advance();

                    match self.current_char {
                        '>' => {
                            token = TokenType::FATARROW;
                            self.advance()
                        }
                        '=' => {
                            token = TokenType::MINUSEQ;
                            self.advance();
                        }
                        '-' => {
                            token = TokenType::MINUSMINUS;
                            self.advance();
                        }
                        _ => {}
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '*' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::MUL;
                    self.advance();

                    match self.current_char {
                        '=' => {
                            token = TokenType::MULEQ;
                            self.advance();
                        }
                        _ => {}
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));

                }
                '/' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::DIV;
                    self.advance();

                    match self.current_char {
                        '/' => {
                            self.advance();
                            while self.current_char != '\n' {
                                self.advance();
                            }
                            self.advance();
                            continue;
                        }
                        '=' => {
                            token = TokenType::DIVEQ;
                            self.advance();
                        }
                        _ => {}
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                
                }
                '(' => {
                    tokens.push(Token::new(TokenType::LPAREN, None, self.pos.clone(), None));
                    self.advance();
                },
                ')' => {
                    tokens.push(Token::new(TokenType::RPAREN, None, self.pos.clone(), None));
                    self.advance();
                }
                ',' => {
                    tokens.push(Token::new(TokenType::COMMA, None,self.pos.clone(), None));
                    self.advance();
                }
                '{' => {
                    tokens.push(Token::new(TokenType::LCURLYBRACKET, None, self.pos.clone(), None));
                    self.advance();
                }
                '}' => {
                    tokens.push(Token::new(TokenType::RCURLYBRACKET, None, self.pos.clone(), None));
                    self.advance();
                }
                ';' => {
                    tokens.push(Token::new(TokenType::SEMICOLON, None, self.pos.clone(), None));
                    self.advance();
                }
                '|' => {
                    let start = self.pos.clone();
                    self.advance();

                    if self.current_char == '|' {
                        self.advance();
                        tokens.push(Token::new(TokenType::OR, None, start, Some(self.pos.clone())));
                    }else{
                        return Err(Box::new(ExpectedCharError::new(String::from("'|' after '|'"), start, self.pos.clone()))); //TODO Expected Char Error
                    }
                   
                }
                '&' => {
                    let start = self.pos.clone();
                    self.advance();

                    if self.current_char == '&' {
                        self.advance();
                        tokens.push(Token::new(TokenType::AND, None, start, Some(self.pos.clone())));
                    }else{
                        return Err(Box::new(ExpectedCharError::new(String::from("'&' after '&'"), start, self.pos.clone()))); //TODO Expected Char Error
                    }
                }
                '!' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::NOT;
                    self.advance();
                    if self.current_char == '=' {
                        token = TokenType::NE;
                        self.advance();
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '=' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::EQ;
                    self.advance();
                    match self.current_char {
                        '=' => {
                            token = TokenType::EE;
                            self.advance();
                        }
                        '>' => {
                            token = TokenType::FATARROW;
                            self.advance();
                        }
                        _ => {}
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '<' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::LT;
                    self.advance();
                    if self.current_char == '=' {
                        token = TokenType::LTE;
                        self.advance();
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '>' => {
                    let start = self.pos.clone();
                    let mut token = TokenType::GT;
                    self.advance();
                    if self.current_char == '=' {
                        token = TokenType::GTE;
                        self.advance();
                    }
                    tokens.push(Token::new(token, None, start, Some(self.pos.clone())));
                }
                '"' => {
                    let mut value: String = String::default();
                    let start = self.pos.clone();
                    self.advance();

                    while self.current_char != '\0' && self.current_char != '"' {
                        value.push(self.current_char);
                        self.advance();
                    }
                    self.advance();
                    tokens.push(Token::new(TokenType::STRING, Some(value), start, Some(self.pos.clone())));
                }
                '[' => {
                    tokens.push(Token::new(TokenType::LBRACKET, None, self.pos.clone(), None));
                    self.advance();
                }
                ']' => {
                    tokens.push(Token::new(TokenType::RBRACKET, None, self.pos.clone(), None));
                    self.advance();
                }
                '.' => {
                    tokens.push(Token::new(TokenType::DOT, None, self.pos.clone(), None));
                    self.advance();
                }
                _ => {
                    let start = self.pos.clone();
                    let error_char = self.current_char.escape_debug().to_string();
                    self.advance();
                    return Err(Box::new(IllegalCharError::new(error_char, start, self.pos.clone())));
                }
            }
        }

        tokens.push(Token::new(TokenType::EOF, None, self.pos.clone(), None));

        Ok((tokens, self.file_name.clone()))
    }
    fn advance(&mut self){
        self.pos.advance(self.current_char);
        if self.pos.idx < self.text.len() as isize {
            self.current_char = self.text.chars().nth(self.pos.idx as usize).unwrap(); 
        } else {
            self.current_char = '\0';
        }
    }
    fn make_number(&mut self) -> Token {
        let start = self.pos.clone();
        let mut num_str: String = String::default();
        let mut dot_count = 0;

        while self.current_char != '\0' && (self.current_char.is_ascii_digit() || self.current_char == '.') {
            if self.current_char == '.' {
                if dot_count == 1 {
                    break;
                }
                dot_count += 1;
                num_str.push(self.current_char);
            }else {
                num_str.push(self.current_char);
            }
            self.advance();
        }
        if dot_count == 0 {
            Token::new(TokenType::INT, Some(num_str), start, Some(self.pos.clone()))
        }else {
            Token::new(TokenType::FLOAT, Some(num_str), start, Some(self.pos.clone()))
        }
    }
    fn make_identifer(&mut self) -> Token {
        let mut id: String = String::default();
        let start = self.pos.clone();

        while self.current_char != '\0' && (self.current_char.is_alphanumeric() || self.current_char == '_') {
            id.push(self.current_char);
            self.advance();
        }
        let token = self.is_keyword(id.clone());
        Token::new(token, Some(id), start, Some(self.pos.clone()))
    }
    fn is_keyword(&mut self, key: String) -> TokenType {
        let keyworld: Vec<String> = vec![
            "import".to_string(),
            "throw".to_string(),
            "return".to_string(),
            "break".to_string(),
            "continue".to_string(),
            "namespace".to_string(),
            "class".to_string(),
            "match".to_string(),
            "for".to_string(),
            "to".to_string(),
            "step".to_string(),
            "while".to_string(),
            "fn".to_string(),
            "if".to_string(),
            "else".to_string(),
            "const".to_string(),
            "let".to_string(),
            "float".to_string(),
            "bool".to_string(),
            "string".to_string(),
            "enum".to_string(),
            "public".to_string(),
            "privite".to_string(),
            "implements".to_string(),
            "extends".to_string(),
            "protected".to_string(),
            "abstract".to_string(),
        ];
        if keyworld.contains(&key) {
            TokenType::KEYWORD
        }else {
            TokenType::IDENTIFIER
        }
    }
}

#[cfg(test)]
mod tests {
   
}