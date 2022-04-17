use std::path::PathBuf;
use std::vec::Vec;
use crate::shared::{Token, Position, TokenType}; 
use crate::errors::LexerError;

#[derive(Debug)]
pub struct Lexer {
    input: String,
    file: PathBuf,
    pos: Position
}

impl Lexer {
    pub async fn parse(input: String, file: PathBuf) -> anyhow::Result<Vec<Token>> {
        let mut parser = Self {
            input,
            file,
            pos: Position::default()
        };

        parser.lex().await
    }
    fn is_keyword(&self, idit: &String) -> bool {
        match idit.as_str() {
            "use" | "let" | "const" | "fn" | "class" | "match" | "if" | "else" | "in" | "for" | "while" | "return" | "break" | "continue" => true,
            _ => false
        }
    }
    async fn lex(&mut self) -> anyhow::Result<Vec<Token>> {
        let mut tokens = vec![];

        let mut iter = self.input.chars().into_iter();
        let mut current = if let Some(value) = iter.next() { value } else { '\0' };

        while current != '\0' {
            match current {
                a if a.is_control() => {}
                a if a.is_whitespace() => {}
                ')' => tokens.push(Token::new(TokenType::RPAREN,self.pos.clone(),self.pos.clone())),
                '(' => tokens.push(Token::new(TokenType::LPAREN,self.pos.clone(),self.pos.clone())),
                ']' => tokens.push(Token::new(TokenType::RBRACKET,self.pos.clone(),self.pos.clone())),
                '[' => tokens.push(Token::new(TokenType::LBRACKET,self.pos.clone(),self.pos.clone())),
                '{' => tokens.push(Token::new(TokenType::LCURLY,self.pos.clone(),self.pos.clone())),
                '}' => tokens.push(Token::new(TokenType::RCURLY,self.pos.clone(),self.pos.clone())),
                '+' => tokens.push(Token::new(TokenType::PLUS,self.pos.clone(),self.pos.clone())),
                '-' => tokens.push(Token::new(TokenType::MINUS,self.pos.clone(),self.pos.clone())),
                '/' => tokens.push(Token::new(TokenType::DIV,self.pos.clone(),self.pos.clone())),
                '*' => tokens.push(Token::new(TokenType::MUL,self.pos.clone(),self.pos.clone())),
                ',' => tokens.push(Token::new(TokenType::COMMA,self.pos.clone(),self.pos.clone())),
                ':' => tokens.push(Token::new(TokenType::COLON,self.pos.clone(),self.pos.clone())),
                ';' => tokens.push(Token::new(TokenType::SEMICOLON,self.pos.clone(),self.pos.clone())),
                '=' => tokens.push(Token::new(TokenType::EQUAL,self.pos.clone(),self.pos.clone())),
                '\n' => {
                    tokens.push(Token::new(TokenType::NEWLINE,self.pos.clone(),self.pos.clone()));
                    self.pos.inc_col();
                    self.pos.set_row(0);
                },
                '"' => {
                    let mut str_value = String::default();
                    let start = self.pos.clone();

                    // ignore " 
                    if let Some(value) = iter.next() {
                        current = value;
                        self.pos.inc_col();
                    } else {
                        current = '\0';
                    }
                  
                    while current != '\0' && current != '"' {
                        str_value.push(current);

                        if let Some(value) = iter.next() {
                            current = value;
                            self.pos.inc_col();
                        } else {
                            current = '\0';
                        }
                    }

                    if let Some(value) = iter.next() {
                        current = value;
                        self.pos.inc_col();
                    } else {
                        current = '\0';
                    }
                    
                    tokens.push(Token::new(TokenType::STRING(str_value),start,self.pos.clone()));

                    continue;
                }
                a if a.is_ascii_digit() => {
                    let mut number = String::default();
                    let start = self.pos.clone();

                    while current != '\0' && current.is_ascii_digit() {
                    
                        number.push(current);

                        if let Some(value) = iter.next() {
                            current = value;
                            self.pos.inc_col();
                        } else {
                            current = '\0';
                        }
                    }

                    match number.parse::<isize>() {
                        Ok(number) => tokens.push(Token::new(TokenType::INTEGER(number),start,self.pos.clone())),
                        Err(_error)=> bail!(LexerError::SyntaxError { 
                            pos: self.pos.clone(), 
                            file: self.file.clone() 
                        })
                    }

                    continue;
                }
                a if a.is_alphabetic() || a == '_' => {
                    let mut id = String::default();
                    let start = self.pos.clone();

                    while current != '\0' {
                        if !(current.is_alphanumeric() || current == '_') {
                            break;
                        }

                        id.push(current);

                        if let Some(value) = iter.next() {
                            current = value;
                            self.pos.inc_col();
                        } else {
                            current = '\0';
                        }
                    }

                    let token_type = if self.is_keyword(&id) {
                        TokenType::KEYWORD(id) 
                    } else {
                        TokenType::IDENTIFIER(id)
                    };

                    tokens.push(Token::new(token_type,start,self.pos.clone()));
                    continue;
                }
                _ => bail!(LexerError::IllegalCharError {
                    pos: self.pos.clone(),
                    illegal: current.escape_debug().to_string(),
                    file: self.file.clone()
                })
            }
            if let Some(value) = iter.next() {
                current = value;
                self.pos.inc_col();
            } else {
                current = '\0';
            }
        }

        tokens.push(Token::new(TokenType::EOF, self.pos.clone(), self.pos.clone()));

        Ok(tokens)
    }
}


#[cfg(test)]
mod tests {
    use super::*;
    fn init() {
        simple_logger::SimpleLogger::new().init().unwrap();
    }

    #[tokio::test]
    async fn test_lexer() {
        init();

        let lexer = Lexer::parse(r#" fn hello() { reutrn 1 + 1; }"#.to_string(),PathBuf::from("<INTERNAL_TEST>")).await;

        match lexer {
            anyhow::Result::Ok(value) => {
                info!("Result {:#?}",value);
            }
            anyhow::Result::Err(value) => {
                trace!("{}",value);
            }
        }
        
    }
}