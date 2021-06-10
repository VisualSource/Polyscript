use crate::shared::{postion::Postion, token::Token};

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