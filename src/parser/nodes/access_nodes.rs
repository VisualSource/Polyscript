use crate::shared::{postion::Postion, token::Token};
use crate::parser::Node;

#[derive(Clone, Debug)]
pub struct PathAccessNode {
    pub start: Postion,
    pub end: Postion,
    pub path: Vec<String>
}
impl PartialEq for PathAccessNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl PathAccessNode {
    pub fn new(path: Vec<Token>, start: Postion, end: Postion) -> Self {
        let mut items: Vec<String> = vec![];
        for elems in path {
            items.push(elems.value.unwrap().clone());
        }
        Self {
            start,
            end,
            path: items
        }
    }
}

#[derive(Clone, Debug)]
pub struct DotAccessNode {
    pub start: Postion,
    pub end: Postion,
    pub path: Vec<String>
}
impl DotAccessNode {
    pub fn new(path: Vec<String>, start: Postion, end: Postion) -> Self{
        Self {
            path,
            start,
            end
        }
    }
}

impl PartialEq for DotAccessNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}


#[derive(Clone, Debug)]
pub struct VarAccessNode {
    pub start: Postion,
    pub end: Postion,
    pub variable: Token,
}
impl PartialEq for VarAccessNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}

impl VarAccessNode {
    pub fn new(variable: Token, start: Postion, end: Postion) -> Self{
        Self{
            variable,
            start,
            end
        }
    }
}


#[derive(Clone, Debug)]
pub struct ListAccessNode {
    pub start: Postion,
    pub end: Postion,
    pub variable_name: String,
    pub index: Box<Node>
}
impl PartialEq for ListAccessNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl ListAccessNode {
    pub fn new(variable: Token, index: Node, start: Postion, end: Postion) -> Self {
        Self {
            start,
            end,
            index: Box::new(index),
            variable_name: variable.value.unwrap()
        }
    }
}