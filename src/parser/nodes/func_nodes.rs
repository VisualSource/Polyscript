use crate::shared::{token::Token, postion::Postion};
use crate::parser::Node;


#[derive(Clone, Debug)]
pub struct FuncCallNode {
    pub start: Postion,
    pub end: Postion,
    pub call: Box<Node>,
    pub arguments: Vec<Box<Node>>
}
impl FuncCallNode {
    pub fn new(call_node: Node, arguments: Vec<Box<Node>>, start: Postion, end: Postion) -> Self {
        Self {
            call: Box::new(call_node),
            arguments,
            start,
            end
        }
    }
}
impl PartialEq for FuncCallNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

#[derive(Clone, Debug)]
pub struct FuncDefNode {
    pub start: Postion,
    pub end: Postion,
    pub function_name: Option<Token>,
    pub body: Box<Node>,
    pub arguments: Vec<Token>
}
impl FuncDefNode {
    pub fn new(name: Option<Token>, arguments: Vec<Token>, body: Node, start: Postion, end: Postion) -> Self {
        Self{
            function_name: name,
            start,
            end,
            body: Box::new(body),
            arguments
        }
    }
}

impl PartialEq for FuncDefNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}