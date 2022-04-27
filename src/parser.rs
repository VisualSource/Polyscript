use std::slice::Iter;
use crate::shared::{ Token, TokenType, Node };
use crate::errors::ParserError;

type ParseResult = anyhow::Result<Node>;

#[derive(Debug)]
pub struct Parser<'a> {
    iterator: Iter<'a,Token>,
    current_token: Token
}

impl Parser<'_> {
    pub fn parse(tokens: &Vec<Token>) -> ParseResult {
        let mut parser = Parser {
            iterator: tokens.iter(),
            current_token: Token::empty()
        };
        parser.run()
    }
    fn run(&mut self) -> ParseResult {
        self.next();
        let lines = self.statemments();

        if !self.current_token.is(TokenType::EOF) {
            bail!(ParserError::SyntaxError {
                pos: self.current_token.start.clone(),
                reason: "Expected end of file"
            })
        }

        lines
    }
    fn scope(&mut self) -> ParseResult {
        let mut statements: Vec<Box<Node>> = vec![];

        if !self.current_token.is(TokenType::LCURLY) {
            bail!(ParserError::SyntaxError { 
                pos: self.current_token.start.clone(), 
                reason: "Expected '{'" 
            })
        }
        self.next();

        while !self.current_token.is(TokenType::RCURLY) {
            if self.current_token.is(TokenType::NEWLINE) {
                self.next();
                continue;
            }

            let line = self.statement();

            if line.is_err() {
                return line;
            }

            if self.current_token.is(TokenType::SEMICOLON) {
                self.next();
            }

            statements.push(Box::new(line.expect("Failed to get node")));
        }

        if !self.current_token.is(TokenType::RCURLY) {
            bail!(ParserError::SyntaxError { 
                pos: self.current_token.start.clone(), 
                reason: "Expected '}'" 
            })
        }
        self.next();

        if statements.is_empty() {
            Ok(Node::Empty)
        } else {
            Ok(Node::Scope { statements: statements })
        }
    }
    fn statemments(&mut self) -> ParseResult {
        let mut statements: Vec<Box<Node>> = vec![];

        loop {
            if self.current_token.is(TokenType::EOF) {
                break;
            }
            if self.current_token.is(TokenType::NEWLINE) {
                self.next();
                continue;
            }

            let line = self.statement();

            if line.is_err() {
                return line;
            }

            if self.current_token.is(TokenType::SEMICOLON) {
                self.next();
            }

            statements.push(Box::new(line.expect("Failed to get node")));
        }

        if statements.is_empty() {
            Ok(Node::Empty)
        } else {
            Ok(Node::Scope { statements: statements })
        }
    
    }
    fn statement(&mut self) -> ParseResult {

        if self.current_token.is_keyword("return") {
            let start = self.current_token.start.clone();
            self.next();

            let expr = self.expr();

            if expr.is_err() {
                return expr;
            }

            return Ok(Node::Return { 
                start, 
                end: self.current_token.end.clone(), 
                node: Box::new(expr.expect("Failed to get node"))
            })
        } 
        self.expr()
    }

    fn atom(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();
        let token = self.current_token.clone();

        match &self.current_token {
            a if a.is(TokenType::INTEGER(0)) => {
                self.next();
                Ok(Node::Integer { start: start.clone(), end: start.clone(), value: token.token.clone() })
            }
            a if a.is(TokenType::STRING(String::default())) => { // string "Value"
                self.next();
                Ok(Node::String { 
                    start: start.clone(), 
                    end: start.clone(), 
                    value: token.token.clone()      
                })
            }
            a if a.is(TokenType::IDENTIFIER(String::default())) => { // varable name, fn name, class name, namespace name, etc
                self.identier()
            }
            a if a.is(TokenType::LCURLY) => { // object def { key: value } 
                self.next();
                let mut properites: Vec<(Token, Node)> = vec![];

                while !self.current_token.is(TokenType::RCURLY) {
                    if properites.len() > 0 {
                        if !self.current_token.is(TokenType::COMMA) {
                            bail!(ParserError::SyntaxError { 
                                pos: self.current_token.start.clone(), 
                                reason: "Expected ','" 
                            })
                        }
                        self.next();
                    }

                    if self.current_token.is(TokenType::NEWLINE) {
                        self.next();
                        continue;
                    } else if self.current_token.is(TokenType::IDENTIFIER(String::default())) {
                        let identifer = self.current_token.clone();
                        self.next();

                        if !self.current_token.is(TokenType::COLON) {
                            bail!(ParserError::SyntaxError { 
                                pos: self.current_token.start.clone(), 
                                reason: "Expected ':'" 
                            })
                        }

                        self.next();

                        let value = self.expr();

                        if value.is_err() {
                            return value;
                        }

                        properites.push((identifer,value.expect("Failed to get node")));
                    } else {
                        bail!(ParserError::SyntaxError { 
                            pos: self.current_token.start.clone(), 
                            reason: "Expected an identier or ','" 
                        })
                    }
                }

                if !self.current_token.is(TokenType::RCURLY) {
                    bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected '}'" 
                    })
                }

                self.next();

                Ok(Node::Object { 
                    props: properites, 
                    start: start, 
                    end: self.current_token.start.clone()
                })

            }
            a if a.is(TokenType::LPAREN) => { // ( )
                self.next();
                let expr = self.expr();
                if self.current_token.is(TokenType::RPAREN) {
                    self.next();
                    expr
                } else {
                    bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected ')'" 
                    })
                }
            }
            a if a.is(TokenType::LBRACKET) => { // list definition [1,1,1]
                let mut values: Vec<Node> = vec![];
                self.next();

               while !self.current_token.is(TokenType::RBRACKET) {
                    let expr = self.expr();

                    if expr.is_err() {
                        return expr;
                    }

                    values.push(expr.expect("Failed to get node"));

                    if !self.current_token.is(TokenType::COMMA) {
                        break;
                    }
                    self.next();
               }

               if !self.current_token.is(TokenType::RBRACKET) {
                   bail!(ParserError::SyntaxError { 
                       pos: self.current_token.start.clone(), 
                       reason: "Expected ']'" 
                    })
               }

               self.next();

               Ok(Node::List { 
                   values: values, 
                   start: start, 
                   end: self.current_token.end.clone() 
                })

            }
            a if a.is_keyword("fn") => {
                self.next();

                let function_name = match &self.current_token.token {
                    TokenType::IDENTIFIER(value) => {
                        let idnt = value.to_owned();
                        self.next();
                        idnt
                    },
                    _ => bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected function name" 
                    })              
                };

                if !self.current_token.is(TokenType::LPAREN) {
                    bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected '('" 
                    })
                }
                self.next();

                let mut args: Vec<TokenType> = vec![];

                while !self.current_token.is(TokenType::RPAREN) {
                    if args.len() > 0 {
                        if !self.current_token.is(TokenType::COMMA) {
                            bail!(ParserError::SyntaxError { 
                                pos: self.current_token.start.clone(), 
                                reason: "Expected ',' after identifer" 
                            })
                        }
                        self.next();
                    }

                    if !self.current_token.is(TokenType::IDENTIFIER(String::default())) {
                        bail!(ParserError::SyntaxError { 
                            pos: self.current_token.start.clone(), 
                            reason: "Expected identifer" 
                        })
                    }

                    args.push(self.current_token.token.to_owned());
                    self.next();
                }

                if !self.current_token.is(TokenType::RPAREN) {
                    bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected ')'" 
                    })
                }
                self.next();

                let body = match self.scope() {
                    Ok(value) => value,
                    Err(err) => bail!(err)
                };

                Ok(Node::Function { 
                    start, 
                    end: self.current_token.start.clone(), 
                    args, 
                    body: Box::new(body), 
                    name: function_name 
                })

            }
            a if a.is_keyword("while") => {
                self.next();

                let expr = self.expr();

                if expr.is_err() {
                    return expr;
                }

                let scope = self.scope();

                if scope.is_err() {
                    return scope;
                }

                Ok(Node::While { 
                    start, 
                    end: self.current_token.end.clone(), 
                    scope: Box::new(scope.expect("Failed to get node")), 
                    expr: Box::new(expr.expect("Failed to get node")) 
                })

            }
            a if a.is_keyword("for") => {
                self.next();
                // KEYWORD IDENTIFIER KEYWORD expr SCOPE

                let element_name = match &self.current_token.token {
                    TokenType::IDENTIFIER(value) => value.clone(),
                    _ => {
                        bail!(ParserError::SyntaxError { 
                            pos: self.current_token.start.clone(), 
                            reason: "Expected a identifer after 'for'" 
                        });
                    }  
                };

                self.next();

                if !self.current_token.is_keyword("in") {
                    bail!(ParserError::SyntaxError { 
                        pos: self.current_token.start.clone(), 
                        reason: "Expected keyword 'in' after identifer" 
                    });
                }

                self.next();

                let iterator = self.expr();

                if iterator.is_err() {
                    return iterator;
                }

                let scope = self.scope();

                if scope.is_err() {
                    return scope;
                }

                Ok(Node::For { 
                    start, 
                    end: self.current_token.end.clone(), 
                    iterator: Box::new(iterator.expect("Failed to get node")), 
                    element_key: element_name, 
                    scope: Box::new(scope.expect("Failed to get node")) 
                })
            }
            a if a.is_keyword("if") => {
                self.next();

                let expr = self.expr();
                if expr.is_err() {
                    return expr;
                }

                let body = self.scope();

                if body.is_err() {
                    return body;
                }

                Ok(Node::If { 
                    start, 
                    end: self.current_token.end.clone(), 
                    expr: Box::new(expr.expect("Failed to get node")), 
                    body: Box::new(body.expect("Failed to get node")) 
                })

            }
            _ => Ok(Node::Empty)
        }
    }
    fn call(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();
        let atom = self.atom();

        if self.current_token.is(TokenType::LPAREN) {
            self.next();
            let mut arguments: Vec<Node> = vec![];

            while !self.current_token.is(TokenType::RPAREN) {
                if arguments.len() > 0 {
                    if !self.current_token.is(TokenType::COMMA) {
                        bail!(ParserError::SyntaxError { 
                            pos: self.current_token.start.clone(), 
                            reason: "Expected ','" 
                        })
                    }
                    self.next();
                }

                let arg = self.expr();

                if arg.is_err() {
                    return arg;
                }

                arguments.push(arg.expect("Failed to get node"));
            }

            self.next();
            
            match atom {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    return Ok(Node::FunctionCall { 
                        identifer: Box::new(node), 
                        arguments: arguments, 
                        start: start, 
                        end: self.current_token.end.clone() 
                    });
                }
            }

        }

        atom
    }

    fn power(&mut self) -> ParseResult {
        //let start = self.current_token.start.clone();
        let left = self.call();

        left
    }

    fn factor(&mut self) -> ParseResult {

        if self.current_token.is(TokenType::PLUS) || self.current_token.is(TokenType::MINUS) {
            let operator = self.current_token.clone();
            self.next();

            let factor = self.factor();
            match factor {
                Err(err) => return Err(err),
                Ok(node) => {
                    return Ok(Node::UnaryOperation { 
                        start: operator.start.clone(),
                        end: self.current_token.end.clone(),
                        operator: operator, 
                        value: Box::new(node), 
                    });
                }
            }
        }
        
        self.power()
    }

    fn term(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();
        let mut left = self.factor();

        while self.current_token.is(TokenType::MUL) || self.current_token.is(TokenType::DIV) {
            let operator = self.current_token.clone();
            self.next();

            let right = self.factor();

            if right.is_err() {
                return right;
            }

            if left.is_err() {
                return left;
            }

            left = Ok(
                Node::BinOperation { 
                    left: Box::new(left.expect("Failed to get node")), 
                    operator: operator.token, 
                    right: Box::new(right.expect("Failed to get node")), 
                    start: start.clone(), 
                    end: self.current_token.end.clone()
                }
            );
        }

        left
    }

    fn arith_expr(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();

        let mut left = self.term();

        while self.current_token.is(TokenType::PLUS) || self.current_token.is(TokenType::MINUS) {
            let operator = self.current_token.clone();
            self.next();

            let right = self.term();

            if left.is_err() {
                return left;
            }
            if right.is_err() {
                return right;
            }

            left = Ok(
                Node::BinOperation { 
                    left: Box::new(left.expect("Failed to get node")), 
                    operator: operator.token, 
                    right: Box::new(right.expect("Failed to get node")), 
                    start: start.clone(), 
                    end: self.current_token.end.clone() 
                }
            );

        }

        left

    }

    fn comp_expr(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();

        if self.current_token.is(TokenType::NOT) {
            let op = self.current_token.clone();
            self.next();
            let com = self.comp_expr();
            if com.is_err() {
                return com;
            }

            return Ok(Node::UnaryOperation { 
                operator: op, 
                value: Box::new(com.expect("Failed to get node")), 
                start, 
                end: self.current_token.end.clone() 
            });
        }

        let mut left = self.arith_expr();

        while self.current_token.is(TokenType::EE) || 
              self.current_token.is(TokenType::GEATERTHENEQ) || 
              self.current_token.is(TokenType::LESSTHENEQ) || 
              self.current_token.is(TokenType::ARROWL) || 
              self.current_token.is(TokenType::ARROWR) {
            let op = self.current_token.clone();
            self.next();

            let right = self.arith_expr();

            if right.is_err() {
                return right;
            }

            if left.is_err() {
                return left;
            }

            left = Ok(Node::BinOperation { 
                left: Box::new(left.expect("Failed to get Node")), 
                operator: op.token, 
                right: Box::new(right.expect("Failed to get Node")), 
                start: start.clone(), 
                end: self.current_token.end.clone() 
            });
        }

        left
    }

    fn expr(&mut self) -> ParseResult {
        if self.current_token.is_keyword("let") || self.current_token.is_keyword("const") {
            let start = self.current_token.start.clone();
            let is_const = self.current_token.is_keyword("const");
            self.next();

            if !self.current_token.is(TokenType::IDENTIFIER(String::default())) {
                bail!(ParserError::SyntaxError { 
                    pos: self.current_token.start.clone(), 
                    reason: "Expected to have identifier after let or const"
                });
            }

            let identifier = self.current_token.clone();
            self.next();

            if !self.current_token.is(TokenType::EQUAL) {
                bail!(ParserError::SyntaxError { 
                    pos: self.current_token.start.clone(), 
                    reason: "Expected '=' after identifier"
                });
            }

            self.next();

            let expr = self.expr();

            if expr.is_err() {
                return expr;
            }

            return Ok(
                Node::VarableDeclarement { 
                    identifer: identifier, 
                    is_const: is_const, 
                    value: Box::new(expr.expect("Failed to get node")), 
                    start: start, 
                    end: self.current_token.end.clone() 
                }
            );
        }


        let left = self.comp_expr();

        // && and ||

        left
    }

    fn identier(&mut self) -> ParseResult {
        let start = self.current_token.start.clone();
        let identifer = self.current_token.clone();

        self.next();
        // list indexing => var[1] or var[var2]
        if self.current_token.is(TokenType::LBRACKET) {
            self.next();

            let indexing_expr = self.expr();

            if !self.current_token.is(TokenType::RBRACKET) {
                bail!(ParserError::SyntaxError { 
                    pos: self.current_token.start.clone(), 
                    reason: "Expected ']'" 
                })
            }
            self.next();

            // TODO: Add list index setting, Ex: a[0] = 3;

            if indexing_expr.is_err() {
                return indexing_expr;
            }

            return Ok(Node::IndexingAccess { 
                identifier: identifer, 
                index: Box::new(indexing_expr.expect("Failed to get node")), 
                start: start, 
                end: self.current_token.end.clone()
            });
        }

        if self.current_token.is(TokenType::EQUAL) {
            let operator = self.current_token.clone();
            self.next();

            return match self.expr() {
                Ok(value) => {
                    Ok(Node::VarableAsignment { 
                        identifer: identifer, 
                        operation: operator, 
                        value: Box::new(value), 
                        start: start, 
                        end: self.current_token.end.clone() 
                    })
                }
                Err(err) => {
                    Err(err)
                }
            };
        }

        Ok(Node::VarableAccess { 
            start: start, 
            end: self.current_token.end.clone(), 
            identifer: identifer.token.clone()
        })

    }

    fn next(&mut self) {
        let value = self.iterator.next();

        if let Some(token) = value {
            self.current_token = token.to_owned();
        }
    }
}