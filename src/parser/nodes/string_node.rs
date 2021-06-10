use crate::shared::{token::Token, postion::Postion};

#[derive(Clone, Debug)]
pub struct StringNode {
    pub start: Postion,
    pub end: Postion,
    pub token: Token,
}
impl PartialEq for StringNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }   
}
impl StringNode {
    pub fn new(token: Token, start: Postion, end: Postion) -> Self {
        Self{
            token,
            start,
            end
        }
    }
}
