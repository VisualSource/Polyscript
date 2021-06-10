use crate::shared::{token::Token, postion::Postion};
use crate::parser::Node;

#[derive(Clone, Debug)]
pub struct BinOpNode {
    pub left_node: Box<Node>,
    pub right_node: Box<Node>,
    pub token: Token,
    pub start: Postion,
    pub end: Postion
}

impl PartialEq for BinOpNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl BinOpNode {
    pub fn new(left_node: Node, op: Token, right_node: Node, start: Postion, end: Postion) -> Self {
        Self {
            left_node: Box::new(left_node),
            right_node: Box::new(right_node),
            token: op,
            start,
            end
        }
    }
}

#[derive(Clone, Debug)]
pub struct UnaryOpNode {
    pub start: Postion,
    pub end: Postion,
    pub token: Token,
    pub node: Box<Node>
}
impl PartialEq for UnaryOpNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl UnaryOpNode {
    pub fn new(token: Token, node: Node, start: Postion, end: Postion) -> Self {
        Self {
            token,
            start,
            end,
            node: Box::new(node)
        }
    }
}