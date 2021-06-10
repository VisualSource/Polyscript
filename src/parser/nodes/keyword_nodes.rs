use crate::shared::{token::Token, postion::Postion};
use crate::parser::Node;

#[derive(Clone, Debug)]
pub struct BreakNode {
    pub start: Postion,
    pub end: Postion,
}

impl BreakNode {
    pub fn new(start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
        }
    }
}
impl PartialEq for BreakNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

#[derive(Clone,Debug)]
pub struct ContinueNode {
    pub start: Postion,
    pub end: Postion,
}

impl ContinueNode {
    pub fn new(start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
        }
    }
}

impl PartialEq for ContinueNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
#[derive(Clone, Debug)]
pub struct ReturnNode {
    pub start: Postion,
    pub end: Postion,
    pub value: Box<Node>
}

impl ReturnNode {
    pub fn new(value: Node, start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
            value: Box::new(value)
        }
    }
}

impl PartialEq for ReturnNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

#[derive(Clone, Debug)]
pub struct ImportNode {
    pub start: Postion,
    pub end: Postion,
    pub path: String
}

impl PartialEq for ImportNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

impl ImportNode {
    pub fn new(path: String, start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
            path
        }
    }
}

#[derive(Clone, Debug)]
pub struct NamespaceNode {
    pub start: Postion,
    pub end: Postion,
    pub statements: Box<Node>,
    pub name: String
}

impl NamespaceNode {
    pub fn new(name: Token, statements: Node, start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
            statements: Box::new(statements),
            name: name.value.unwrap()
        }
    }
}

impl PartialEq for NamespaceNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

#[derive(Clone, Debug)]
pub struct ForNode {
    pub start: Postion,
    pub end: Postion,
    pub var_name: Token,
    pub start_value: Box<Node>,
    pub end_value: Box<Node>,
    pub step: Box<Node>,
    pub body: Box<Node>
 
}

impl PartialEq for ForNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

impl ForNode {
    pub fn new(var_name: Token, start_value: Node, end_value: Node, step: Node, body: Node, start: Postion, end: Postion) -> Self {
        Self{
            start,
            end,
            var_name,
            start_value: Box::new(start_value),
            end_value: Box::new(end_value),
            step: Box::new(step),
            body: Box::new(body)
        }
    }
}

#[derive(Clone, Debug)]
pub struct IfNode {
    pub start: Postion,
    pub end: Postion,
    pub statements: Vec<(Box<Node>,Box<Node>)>,
    pub else_statment: Option<Box<Node>>
}
impl IfNode {
    pub fn new(statements: Vec<(Box<Node>,Box<Node>)>, else_statment: Option<Box<Node>>, start: Postion, end: Postion) -> Self {
        Self{
            statements,
            else_statment,
            start,
            end
        }
    }
}

impl PartialEq for IfNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}



#[derive(Clone, Debug)]
pub struct WhileNode {
    pub start: Postion,
    pub end: Postion,
    pub condition: Box<Node>,
    pub body: Box<Node>
}
impl WhileNode {
    pub fn new(condition: Node, body: Node, start: Postion, end: Postion) -> Self {
        Self{
            condition: Box::new(condition),
            body: Box::new(body),
            start,
            end
        }
    }
}

impl PartialEq for WhileNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

pub mod node_match {
    use super::*;

    #[derive(Clone, Debug)]
    pub struct MatchExpr {
        pub scoped: bool,
        pub check: Box<Node>,
        pub expr: Box<Node>
    } 
    impl MatchExpr {
        pub fn new(check: Node, expr: Node, scoped: bool) -> Self{
            Self{
                check: Box::new(check),
                expr: Box::new(expr),
                scoped
            }
        }
    }

    #[derive(Clone, Debug)]
    pub struct MatchNode {
        pub start: Postion,
        pub end: Postion,
        pub expr: Box<Node>,
        pub matches: Vec<MatchExpr>
    }
    impl MatchNode {
        pub fn new(expr: Node, matches: Vec<MatchExpr>, start: Postion, end: Postion) -> Self {
            Self {
                expr: Box::new(expr),
                matches,
                start,
                end
            }
        }
    }

    impl PartialEq for MatchNode  {
        fn eq(&self, _other: &Self) -> bool {
            false
        }   
    }

}

