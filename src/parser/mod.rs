pub mod nodes;
mod node;
use crate::shared::{token::Token, token_type::TokenType, postion::Postion };
use crate::errors::{error::Error, InvaildSyntaxError};
pub use node::Node;

use nodes::*;

#[derive(Debug)]
pub struct Parser {
    pub current_token: Token,
    pub tokens: Vec<Token>,
    pub tok_idx: isize
}

impl Parser {
    fn new_syntax_error<T>(&mut self, details: T, start: Postion) -> Box<dyn Error> where T: ToString {
        return Box::new(InvaildSyntaxError::new(details.to_string(), start, self.current_token.end.clone()))
    }
    pub fn new(tokens: Vec<Token>) -> Self {
        let mut a = Self {
            tok_idx: -1,
            current_token: Token::new(TokenType::NEWLINE, None, Postion::default(), None),
            tokens,
        };
        a.advance();
        a
    }
    pub fn parse(&mut self) -> Result<Node, Box<dyn Error>> {
        let res = self.statements();

        if !self.current_token.is(&TokenType::EOF) && !res.is_err() {
            return Err(self.new_syntax_error("Expected End of file", self.current_token.start.clone()));
        }

        match res {
            Err(e) => {
                return Err(e);
            }
            Ok(e) => {
                return Ok(e)
            }
        }

    }
    fn advance(&mut self) {
        self.tok_idx += 1;
        if self.tok_idx < self.tokens.len() as isize {
            self.current_token = self.tokens[self.tok_idx as usize].clone();
        }
    }
    fn statements(&mut self) -> Result<Node, Box<dyn Error>> {
        let mut statements: Vec<Node> = vec![];
        let start = self.current_token.start.clone();

        while self.current_token.is(&TokenType::NEWLINE) {
            self.advance();
        }

        let out = self.statement();

        match out {
            Err(e)=>{
                return Err(e);
            }
            Ok(e)=>{
                statements.push(e);
            }
        }
       

        let mut more_statments = true;

        loop {
            let mut new_lines = 0;
            while self.current_token.is(&TokenType::NEWLINE) || self.current_token.is(&TokenType::SEMICOLON) {
                self.advance();
                new_lines += 1;
            }
            if new_lines == 0 {
                more_statments = false;
            }
            if !more_statments { break; }
            let reset = self.tok_idx;
            let statement = self.statement();
            match statement {
                Err(e) => {
                    return Err(e);
                }
                Ok(e) => {
                    if e == Node::Empty {
                        self.tok_idx = reset;
                        self.current_token = self.tokens[self.tok_idx as usize].clone();
                        more_statments = false;
                        continue;
                    }
                    statements.push(e);
                }
            }
        }

        if statements.is_empty() {
            Ok(Node::Empty)
        }else {
            Ok(Node::new_list_node(statements, start, self.current_token.end.clone()))
        }
    }
    fn statement(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();

        if self.current_token.is_keyword(&"return") {
            self.advance();

            let reset = self.tok_idx;
            let statment = self.expr();
            match statment {
                Err(e) => {
                    return Err(e);
                }
                Ok(e) => {
                    if e == Node::Empty {
                        self.tok_idx = reset;
                        self.current_token = self.tokens[self.tok_idx as usize].clone();
                    }
                    return Ok(Node::new_return_node(e, start, self.current_token.end.clone()));
                }
            }
        }else if self.current_token.is_keyword(&"continue") {
            self.advance();
            return Ok(Node::new_continue_node(start, self.current_token.end.clone()));
        } else if self.current_token.is_keyword(&"break"){
            self.advance();
            return Ok(Node::new_break_node(start, self.current_token.end.clone()));
        }
        
        self.expr()
    }

    fn expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();

        if self.current_token.is_keyword(&"let") || self.current_token.is_keyword(&"const") {
            let is_constant = !self.current_token.is_keyword(&"const");
            self.advance();

            if !self.current_token.is(&TokenType::IDENTIFIER) {
                return Err(self.new_syntax_error("Expected Identifier", start));
            }

            let var_name = self.current_token.clone();
            self.advance();

            let mut var_type: Option<Token> = None;

            if self.current_token.is(&TokenType::COLON) {
                self.advance();
                if self.current_token.is_keyword(&"let") || self.current_token.is_keyword(&"float") || self.current_token.is_keyword(&"string") || self.current_token.is_keyword(&"bool") {
                    var_type = Some(self.current_token.clone());
                    self.advance();
                }else {
                    return Err(self.new_syntax_error("Expected type", start));
                }
            }


            if !self.current_token.is(&TokenType::EQ) {
                return Err(self.new_syntax_error("Expected '='", start));
            }

            self.advance();

            let expr = self.expr();

            match expr {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) =>{
                    return Ok(Node::new_var_assign_node(var_name, is_constant, node, start, self.current_token.end.clone(), var_type));
                }
            }

            
        }
       
        let mut left = self.comp_expr();

        while self.current_token.is(&TokenType::AND) || self.current_token.is(&TokenType::OR) { 
                let op = self.current_token.clone();
                self.advance();
    
                let right = self.comp_expr();

                let left_node = match &left {
                    Err(err) => {
                        return Err(Box::new(InvaildSyntaxError::new(err.get_details(), err.get_start(), err.get_end())));
                    }
                    Ok(node) => node
                };

                match right {
                    Err(err) => {
                        return Err(err); 
                    }
                    Ok(right_node) => {
                        left = Ok(Node::new_bin_op_node(left_node.clone(), op, right_node, start.clone(), self.current_token.end.clone()));
                    }
                }
        }


        left

        
    }
    fn comp_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if self.current_token.is(&TokenType::NOT) {
            let op = self.current_token.clone();
            self.advance();
            let node = self.comp_expr();
            match node {
                Err(err) => {
                    return Err(err);
                }
                Ok(out) => {
                    return Ok(Node::new_unary_op_node(op, out, start, self.current_token.end.clone()));
                }
            }
        }

        let mut left = self.arith_expr();

        while self.current_token.is(&TokenType::EE) || self.current_token.is(&TokenType::GT) || 
              self.current_token.is(&TokenType::NE) || self.current_token.is(&TokenType::LT) || 
              self.current_token.is(&TokenType::LTE) || self.current_token.is(&TokenType::GTE) {
                let op = self.current_token.clone();
                self.advance();
                let right = self.arith_expr();
                match left {
                    Err(err)=>{
                        return Err(err);
                    }
                    Ok(left_node) =>{
                        match right {
                            Err(err)=>{
                                return Err(err);
                            }
                            Ok(right_node) =>{
                                left = Ok(Node::new_bin_op_node(left_node, op, right_node, start.clone(), self.current_token.end.clone()));
                            }
                        }
                    }
                }
               
        }

        left

    }
    fn arith_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let mut left = self.term();

        while self.current_token.is(&TokenType::PLUS) || self.current_token.is(&TokenType::MINUS) {
            let op = self.current_token.clone();
            self.advance();

            let right = self.term();

            let left_node = match &left {
                Err(err) => {
                    return Err(Box::new(InvaildSyntaxError::new(err.get_details(), err.get_start(), err.get_end())));
                }
                Ok(node) => node
            };

            match right {
                Err(err) => {
                    return Err(err); 
                }
                Ok(right_node) => {
                    left = Ok(Node::new_bin_op_node(left_node.clone(), op, right_node, start.clone(), self.current_token.end.clone()));
                }
            }

        }


        left
    }
    fn term(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let mut left = self.factor();

        while self.current_token.is(&TokenType::MUL) || self.current_token.is(&TokenType::DIV) {
            let op = self.current_token.clone();
            self.advance();

            let right = self.factor();

            let left_node = match &left {
                Err(err) => {
                    return Err(Box::new(InvaildSyntaxError::new(err.get_details(), err.get_start(), err.get_end())));
                }
                Ok(node) => node
            };

            match right {
                Err(err) => {
                    return Err(err); 
                }
                Ok(right_node) => {
                    left = Ok(Node::new_bin_op_node(left_node.clone(), op, right_node, start.clone(), self.current_token.end.clone()));
                }
            }
        }

        left

    }
    fn factor(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let tok = self.current_token.clone();

        if tok.is(&TokenType::PLUS) || tok.is(&TokenType::MINUS) {
            self.advance();
            let factor = self.factor();
            match factor {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    return Ok(Node::new_unary_op_node(tok, node, start, self.current_token.end.clone()));
                }
            }
        }

        self.power()

    }
    fn power(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let mut left = self.call();

        while self.current_token.is(&TokenType::POWER) {
            let op = self.current_token.clone();
            self.advance();
            let right = self.factor();

            let left_node = match &left {
                Err(err) => {
                    return Err(Box::new(InvaildSyntaxError::new(err.get_details(), err.get_start(), err.get_end())));
                }
                Ok(node) => node
            };

            match right {
                Err(err) => {
                    return Err(err); 
                }
                Ok(right_node) => {
                    left = Ok(Node::new_bin_op_node(left_node.clone(), op, right_node, start.clone(), self.current_token.end.clone()));
                }
            }
        }

        left
    }
    fn call(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let atom = self.atom();

        if self.current_token.is(&TokenType::LPAREN) {
            self.advance();
            let mut arguments: Vec<Box<Node>> = vec![];
            if self.current_token.is(&TokenType::RPAREN) {
                self.advance();
            } else {
                let expr = self.expr();
                match expr {
                    Err(err) => {
                        return Err(err);
                    }
                    Ok(node) => {
                        arguments.push(Box::new(node));
                    }
                }
                while self.current_token.is(&TokenType::COMMA) {
                    self.advance();
                    let expr = self.expr();
                    match expr {
                        Err(err) => {
                            return Err(err);
                        }
                        Ok(arg) => {
                            arguments.push(Box::new(arg));
                        }
                    }
                }

                if !self.current_token.is(&TokenType::RPAREN) {
                    return Err(self.new_syntax_error("Expected identifier or ','", start));
                }
                self.advance();
            }

            match atom {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    return Ok(Node::new_func_call_node(node, arguments, start, self.current_token.end.clone()));
                }
            }
        }


        atom

    }
    fn atom(&mut self) -> Result<Node, Box<dyn Error>>  {
        let start = self.current_token.start.clone();
        let tok = self.current_token.clone();

        match tok {
            a if a.is(&TokenType::INT) || a.is(&TokenType::FLOAT) => {
                self.advance();
                Ok(Node::new_number_node(a, start, self.current_token.end.clone()))
            }
            a if a.is(&TokenType::STRING) => {
                self.advance();
                Ok(Node::new_string_node(a, start, self.current_token.end.clone()))
            }
            a if a.is(&TokenType::IDENTIFIER) => {
                self.identifer(true)
            }
            a if a.is(&TokenType::LCURLYBRACKET) => {
                self.advance();
                let mut props: Vec<(Token, Node)> = vec![];

                while !self.current_token.is(&TokenType::RCURLYBRACKET) {

                    if self.current_token.is(&TokenType::NEWLINE) {
                        self.advance();
                        continue;
                    }else if self.current_token.is(&TokenType::IDENTIFIER) {
                        let name = self.current_token.clone();
                        self.advance();
                        if !self.current_token.is(&TokenType::COLON) {
                            return Err(Box::new(InvaildSyntaxError::new("Expected ':'".to_string(), start, self.current_token.end.clone())));
                        }
                        self.advance();

                        let value = self.expr();

                        if self.current_token.is(&TokenType::COMMA) {
                            self.advance();
                        }

                        match value {
                            Err(err) => {
                                return Err(err)
                            }
                            Ok(node) => {
                                props.push((name,node));
                            }
                        }

                    } else {
                        return Err(Box::new(InvaildSyntaxError::new("Expected a Identifier or ','".to_string(), start, self.current_token.end.clone())));
                    }

                }

                if !self.current_token.is(&TokenType::RCURLYBRACKET) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
                }

                self.advance();

                return Ok(Node::new_object_node(props,start,self.current_token.end.clone()))
            }
            a if a.is(&TokenType::LPAREN) => {
                self.advance();
                let expr  = self.expr();
                if self.current_token.is(&TokenType::RPAREN) {
                    self.advance();
                    expr 
                }else {
                    Err(Box::new(InvaildSyntaxError::new("Expected ')'".to_string(), start, self.current_token.end.clone())))
                }
            }
            a if a.is(&TokenType::LBRACKET) => {
                self.list_expr()
            }
            a if a.is_keyword(&"if") => {
                self.if_expr()
            }
            a if a.is_keyword(&"for") => {
                self.for_expr()
            }
            a if a.is_keyword(&"while") => {
                self.while_expr()
            }
            a if a.is_keyword(&"fn") => {
                self.func_def()
            }
            a if a.is_keyword(&"enum") => {
                self.enum_def()
            }
            a if a.is_keyword(&"namespace") => {
                self.namespace_def()
            }
            a if a.is_keyword(&"import") => {
                self.import_def()
            }
            a if a.is_keyword(&"match") => {
                self.match_def()
            }
            _ => Ok(Node::Empty)
        }
    }
    fn identifer(&mut self, check_eq: bool) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let tok = self.current_token.clone();
        self.advance();

        if check_eq && self.current_token.is(&TokenType::LBRACKET) {
            self.advance();

            let index = self.expr();

            if !self.current_token.is(&TokenType::RBRACKET) {
                return Err(Box::new(InvaildSyntaxError::new("Expected ']'".to_string(), start, self.current_token.end.clone())));
            }
            self.advance();

            match index {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    return Ok(Node::new_list_access_node(tok,node,start,self.current_token.end.clone()));
                }
            }
    
        }else if self.current_token.is(&TokenType::PATHSEPERATOR) {
            let mut path: Vec<Token> = vec![];
            path.push(tok.clone());

            while self.current_token.is(&TokenType::PATHSEPERATOR) {
                self.advance();
                if !self.current_token.is(&TokenType::IDENTIFIER) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected Identifier".to_string(), start, self.current_token.end.clone())));
                }
                path.push(self.current_token.clone());
                self.advance();
            }

            return Ok(Node::new_path_access_node(path, start, self.current_token.end.clone()))
        } else if check_eq && (self.current_token.is(&TokenType::EQ) || self.current_token.is(&TokenType::PLUSEQ) || self.current_token.is(&TokenType::MINUSEQ) || self.current_token.is(&TokenType::MULEQ) || self.current_token.is(&TokenType::DIVEQ)){
            let op = self.current_token.clone();
            self.advance();

            let value = self.expr();

            match value {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    return Ok(Node::new_var_reasign_node(tok, op, node, start, self.current_token.end.clone()));
                }
            }
        } else if self.current_token.is(&TokenType::DOT) {
            let mut path: Vec<String> = vec![];
            path.push(tok.value.as_ref().unwrap().clone());
            while self.current_token.is(&TokenType::DOT) {
                self.advance();

                if !self.current_token.is(&TokenType::IDENTIFIER) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected Identifier".to_string(),start,self.current_token.end.clone())));
                }

                path.push(self.current_token.value.as_ref().unwrap().clone());
                self.advance();
            }

            return Ok(Node::new_dot_access_node(path, start, self.current_token.end.clone()))
        }


        Ok(Node::new_var_access_node(tok, start, self.current_token.end.clone()))
    }
    fn list_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let mut nodes: Vec<Node> = vec![]; 
        let start = self.current_token.start.clone();

        if !self.current_token.is(&TokenType::LBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '['".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        if self.current_token.is(&TokenType::RBRACKET) {
            self.advance();
        }else {
            let expr = self.expr();
            match expr {
                Err(err) => {
                    return Err(err);
                }
                Ok(node) => {
                    nodes.push(node);
                }
            }

            while self.current_token.is(&TokenType::COMMA) {
                self.advance();
                let expr = self.expr();
                match expr {
                    Err(err) => {
                        return Err(err);
                    }
                    Ok(node) => {
                        nodes.push(node);
                    }
                }
            }
            if !self.current_token.is(&TokenType::RBRACKET) {
                return Err(Box::new(InvaildSyntaxError::new("Expected ']'".to_string(), start, self.current_token.end.clone())));
            }
            self.advance();
        }

        Ok(Node::new_list_node(nodes, start, self.current_token.end.clone()))
    }
    fn if_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let mut statments: Vec<(Box<Node>,Box<Node>)> = vec![];
        let mut else_statment: Option<Box<Node>> = None; 

        if !self.current_token.is_keyword(&String::from("if")) {
            return Err(self.new_syntax_error("Expected keyword 'if'", start));
        }

        self.advance();

        let condition = self.expr();

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(self.new_syntax_error("Expected '{'", start));
        }

        self.advance();

        let body = self.statements();

        match body {
            Err(err) => {
                return Err(err);
            }
            Ok(node) => {
                if node == Node::Empty {
                    self.advance();
                }else {
                    match condition {
                        Err(e) => {
                            return Err(e);
                        }
                        Ok(cond) => {
                            statments.push((Box::new(cond),Box::new(node)));
                        }
                    }
                }
            }
        }

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(self.new_syntax_error("Expected '}'", start));
        }

        self.advance();

        while self.current_token.is_keyword(&String::from("else")) {
            self.advance();

            if self.current_token.is_keyword(&String::from("if")) {
                self.advance();
                let condition = self.expr();
                if !self.current_token.is(&TokenType::LCURLYBRACKET) {
                    return Err(self.new_syntax_error("Expected '{'", start));
                }
                self.advance();
                let body = self.statements();
                match body {
                    Err(err) => {
                        return Err(err);
                    }
                    Ok(node) => {
                        if node == Node::Empty {
                            self.advance();
                        }else {
                            match condition {
                                Err(e) => {
                                    return Err(e);
                                }
                                Ok(cond) => {
                                    statments.push((Box::new(cond),Box::new(node)));
                                }
                            }
                        }
                    }
                }

                if !self.current_token.is(&TokenType::RCURLYBRACKET) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
                }

                self.advance();

            } else if self.current_token.is(&TokenType::LCURLYBRACKET) {
                self.advance();
                let else_s = self.statements();

                match else_s {
                    Err(err) => {
                        return Err(err);
                    }
                    Ok(statment) => {
                        else_statment = Some(Box::new(statment));
                    }
                }

                if !self.current_token.is(&TokenType::RCURLYBRACKET) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
                }

                self.advance();


               
            } else {
               return Err(Box::new(InvaildSyntaxError::new("Expected 'if' or '{'".to_string(), start, self.current_token.end.clone()))); 
            }
          
        }

        Ok(Node::new_if_node(statments, else_statment, start, self.current_token.end.clone()))
        
    }
    fn for_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();

        if !self.current_token.is_keyword(&"for") {
            return Err(self.new_syntax_error("Expected 'for'", start));
        }
        self.advance();

        if !self.current_token.is(&TokenType::IDENTIFIER) {
            return Err(self.new_syntax_error("Expected Identifier", start));
        }

        let var_name = self.current_token.clone();
        self.advance();

        if !self.current_token.is(&TokenType::EQ) {
            return Err(self.new_syntax_error("Expected '='", start));
        }
        self.advance();

        let start_value = self.expr();

        if !self.current_token.is_keyword(&"to") {
            return Err(self.new_syntax_error("Expected 'to'", start));
        }

        self.advance();

        let end_value = self.expr();
        
        let mut step_value = Node::new_number_node(Token::new(TokenType::INT, Some(String::from("1")), start, None), start.clone(), start.clone());
        if self.current_token.is_keyword(&"step") {
            self.advance();
            let expr = self.expr();
            match expr {
                Err(err) => {
                    return Err(err);
                }
                Ok(v) => {
                    step_value = v;
                }
            }
        }

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        let body = self.statements();

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();


        match start_value {
            Err(err_s) => Err(err_s),
            Ok(start_node) => {
                match end_value {
                    Err(err_e) => return Err(err_e),
                    Ok(end_node) => {
                       match body {
                            Err(err_b) => Err(err_b),
                            Ok(body_node) => {
                                return Ok(Node::new_for_node(var_name, start_node, end_node, step_value, body_node, start, self.current_token.end.clone()))
                            }
                        }
                    }
                }
            }
        }
       
    }
    fn while_expr(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&String::from("while")) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'while'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        let condition = self.expr();
        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        let body = self.statements();

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{}".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        match condition {
            Err(err_c) => Err(err_c),
            Ok(condition_node) => {
                match body {
                    Err(err_b) => Err(err_b),
                    Ok(body_node) => {
                        Ok(Node::new_while_node(condition_node, body_node, start, self.current_token.end.clone()))
                    }
                }
            }
        }
    }
    fn func_def(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&String::from("fn")) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'fn'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();
        let mut var_name: Option<Token> = None;

        if self.current_token.is(&TokenType::IDENTIFIER) {
            var_name = Some(self.current_token.clone());
            self.advance();
        } 

        if !self.current_token.is(&TokenType::LPAREN) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '('".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();


        let mut argument_names: Vec<Token> = vec![];
        
        if self.current_token.is(&TokenType::IDENTIFIER) {
            argument_names.push(self.current_token.clone());
            self.advance();

            while self.current_token.is(&TokenType::COMMA) {
                self.advance();
                if !self.current_token.is(&TokenType::IDENTIFIER) {
                    return Err(Box::new(InvaildSyntaxError::new("Expected Identifier".to_string(), start, self.current_token.end.clone())));
                }
                argument_names.push(self.current_token.clone());
                self.advance();
            }
        }
        if !self.current_token.is(&TokenType::RPAREN) {
            return Err(Box::new(InvaildSyntaxError::new("Expected ')'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        let body = self.statements();

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        match body {
            Err(err) => Err(err),
            Ok(node) => {
                Ok(Node::new_func_def_node(var_name,argument_names,node,start,self.current_token.end.clone()))
            }
        }
    }
    fn enum_def(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&String::from("enum")) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'enum'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();
        if !self.current_token.is(&TokenType::IDENTIFIER) {
            return Err(Box::new(InvaildSyntaxError::new("Expected Identifier".to_string(), start, self.current_token.end.clone())));
        }

        let name = self.current_token.value.as_ref().unwrap().clone();
        self.advance();

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        let mut keys: Vec<Token> = vec![];

        while !self.current_token.is(&TokenType::RCURLYBRACKET) {
            if self.current_token.is(&TokenType::NEWLINE) {
                self.advance();
                continue;
            }else if self.current_token.is(&TokenType::IDENTIFIER) {
                keys.push(self.current_token.clone());
                self.advance();
                if self.current_token.is(&TokenType::COMMA) {
                    self.advance();
                }
            }else {
                return Err(Box::new(InvaildSyntaxError::new("Expected identifier".to_string(), start, self.current_token.end.clone())));
            }
        }

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        Ok(Node::new_enum_node(name, keys, start, self.current_token.end.clone()))
    }
    fn namespace_def(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&String::from("namespace")) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'namespace'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        if !self.current_token.is(&TokenType::IDENTIFIER) {
            return Err(Box::new(InvaildSyntaxError::new("Expected Identifier".to_string(), start, self.current_token.end.clone())));
        }

        let name = self.current_token.clone();

        self.advance();

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '{'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        let body = self.statements();

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        match body {
            Err(err) => Err(err),
            Ok(node) => {
                Ok(Node::new_namespace_node(name, node, start, self.current_token.end.clone()))
            }
        }
    }
    fn import_def(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&String::from("import")) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'import'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        if !self.current_token.is(&TokenType::STRING) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'string'".to_string(), start, self.current_token.end.clone())));
        }
        let path = self.current_token.value.as_ref().unwrap().clone();
        self.advance();

        if !self.current_token.is(&TokenType::SEMICOLON) {
            return Err(Box::new(InvaildSyntaxError::new("Expected 'string'".to_string(), start, self.current_token.end.clone())));
        }

        self.advance();

        Ok(Node::new_import_node(path, start, self.current_token.end.clone()))

    }
    fn match_def(&mut self) -> Result<Node, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        if !self.current_token.is_keyword(&"match") {
            return Err(self.new_syntax_error("Expected 'match'", start));
        }
        self.advance();

        let expr = self.expr();

        if !self.current_token.is(&TokenType::LCURLYBRACKET) {
            return Err(self.new_syntax_error("Expected '{'", start));
        }
        self.advance();

        let mut matches: Vec<keyword_nodes::node_match::MatchExpr> = vec![];
        
        while !self.current_token.is(&TokenType::RCURLYBRACKET) {
            if self.current_token.is(&TokenType::NEWLINE) {
                self.advance();
                continue;
            }
            match self.make_match_expr() {
                Err(e) => {
                    return Err(e);
                }
                Ok(value) => {
                    matches.push(value);
                }
            }
            if self.current_token.is(&TokenType::COMMA){
                self.advance();
            }
        }

        while self.current_token.is(&TokenType::NEWLINE) {
            self.advance();
        }

        if !self.current_token.is(&TokenType::RCURLYBRACKET) {
            return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
        }
        self.advance();

        match expr {
            Err(err) => Err(err),
            Ok(node) => {
                Ok(Node::new_match_mode(node, matches, start, self.current_token.end.clone()))
            }
        }

    }

    fn make_match_expr(&mut self) -> Result<keyword_nodes::node_match::MatchExpr, Box<dyn Error>> {
        let start = self.current_token.start.clone();
        let check_value; 
        match &self.current_token {
            a if a.is(&TokenType::INT) || a.is(&TokenType::FLOAT) => { 
                check_value = Node::new_number_node(a.clone(), start, a.end.clone());
                self.advance();
            }
            a if a.is(&TokenType::STRING) => {
                check_value = Node::new_string_node(a.clone(), start, a.end.clone());
                self.advance();
            }
            a if a.is(&TokenType::IDENTIFIER) => {
                if a.matches(&TokenType::IDENTIFIER,&"_") {
                    check_value = Node::MatchCatchAll;
                    self.advance();
                } else {
                    match self.identifer(false) {
                        Err(err) => {
                            return Err(err);
                        }
                        Ok(node) => {
                            check_value = node;
                        }
                    }
                }
            }
            _ => {
                return Err(self.new_syntax_error("Expected INT|FLOAT|STRING|ENUM or '_' ", start));
            }
        }

        if !self.current_token.is(&TokenType::FATARROW) {
            return Err(self.new_syntax_error("Expected '=>'", start));
        }
        self.advance();

        let mut scoped = false;
        let body;
        if self.current_token.is(&TokenType::LCURLYBRACKET) {
            scoped = true;
            self.advance();

            body = self.statements();

            if !self.current_token.is(&TokenType::RCURLYBRACKET){
                return Err(Box::new(InvaildSyntaxError::new("Expected '}'".to_string(), start, self.current_token.end.clone())));
            }
            self.advance();
        }
        else{
            body = self.statement();
        }

        match body {
            Err(err) => Err(err),
            Ok(node) => {
                if !scoped && node == Node::Empty {
                    return Err(self.new_syntax_error("expected expression", start));
                }
                Ok(keyword_nodes::node_match::MatchExpr::new(check_value, node, scoped))
            }
        }
    
    }
}


#[cfg(test)]
mod tests {
    use crate::Lexer;
    use super::*;
    fn test_tokens<T>(text: T) -> Vec<Token> where T: ToString {
       let mut lex = Lexer::new(text.to_string(), String::from("<TEST INPUT>"));
       if let Ok(tokens) = lex.make_tokens() {
          tokens.0
       }else {
           panic!("Text Lexer Error");
       }
    }
    #[test]
    fn parser_advance_test(){
        let mut parser = Parser::new(test_tokens("let a = 1;"));

        assert_eq!(parser.tok_idx,0);
        assert_eq!(parser.current_token.matches(&TokenType::KEYWORD, &String::from("let")),true);

        println!("{:#?}",parser);

        parser.advance();

        assert_eq!(parser.tok_idx,1);
        assert_eq!(parser.current_token.matches(&TokenType::IDENTIFIER, &String::from("name")),true);

        println!("{:#?}",parser);

    }

    #[test]
    fn parser_atom_test(){
        let mut parser_integer = Parser::new(test_tokens("1"));

        match parser_integer.atom() {
            Err(_err) => {
                panic!("Parser Integer Error");
            }
            Ok(node) => {
                match node {
                    Node::NumberNode(value) => {
                        assert_eq!(value.token.matches(&TokenType::INT,&"1".to_string()),true);
                    }
                    _ => {
                        panic!("Did Not resive a number node from a number");
                    }
                }
            }
        }
        let mut parser_float = Parser::new(test_tokens("1.1"));
        match parser_float.atom() {
            Err(_err) => {
                panic!("Parser Float Error");
            }
            Ok(node) => {
                match node {
                    Node::NumberNode(value) => {
                        assert_eq!(value.token.matches(&TokenType::FLOAT,&"1.1".to_string()),true);
                    }
                    _ => {
                        panic!("Did Not resive a number node from a float");
                    }
                }
            }
        }

        let mut parser_string = Parser::new(test_tokens("\"TEST\""));
        match parser_string.atom() {
            Err(_err) => {
                panic!("Parser String Error");
            }
            Ok(node) => {
                match node {
                    Node::StringNode(value) => {
                        assert_eq!(value.token.matches(&TokenType::STRING,&"TEST".to_string()),true);
                    }
                    _ => {
                        panic!("Did Not get a string node");
                    }
                }
            }
        }

    }


    #[test]
    fn full_test(){
        let mut parser = Parser::new(test_tokens("let a = 1; if a == 1 { print(\"World\") }"));
        match parser.parse() {
            Err(err) =>{
                println!("{:#?}",err);
            }
            Ok(node) => {
                println!("{:#?}",node);
            }
        }
    }
}

