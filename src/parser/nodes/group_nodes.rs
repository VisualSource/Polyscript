use crate::shared::{postion::Postion, token::Token};
use crate::parser::Node;

#[derive(Clone, Debug)]
pub struct ListNode {
    pub start: Postion,
    pub end: Postion,
    pub elements: Vec<Node>
}
impl PartialEq for ListNode {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl ListNode {
    pub fn new(elements: Vec<Node>, start: Postion, end: Postion) -> Self {
        Self {
            elements,
            start,
            end
        }
    }
}

#[derive(Clone, Debug)]
pub struct ObjectNode {
    pub start: Postion,
    pub end: Postion,
    pub values: Vec<(Token,Node)>
}
impl PartialEq for ObjectNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl ObjectNode {
    pub fn new(values: Vec<(Token,Node)>, start: Postion, end: Postion) -> Self{
        Self{
            start,
            end,
            values
        }
    }
}

#[derive(Clone, Debug)]
pub struct EnumNode {
    pub start: Postion,
    pub end: Postion,
    pub keys: Vec<Token>,
    pub name: String
}

impl EnumNode {
    pub fn new(name: String, values: Vec<Token>,start: Postion, end: Postion) -> Self {
        Self{
            name,
            keys: values,
            start,
            end
        }
    }
}

impl PartialEq for EnumNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}