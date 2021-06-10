use crate::shared::{token::Token, postion::Postion};
use crate::parser::Node;

#[derive(Clone, Debug)]
pub struct VarAssignNode {
    pub start: Postion,
    pub end: Postion,
    pub variable_type: Option<Token>,
    pub token: Token,
    pub node: Box<Node>,
    pub constant: bool
}
impl PartialEq for VarAssignNode  {
    fn eq(&self, _other: &Self) -> bool {
        false
    }
    
}

impl VarAssignNode {
    pub fn new(token: Token, constant: bool, node: Node, start: Postion, end: Postion, var_type: Option<Token>) -> Self {
        Self {
            token,
            constant,
            node: Box::new(node),
            start,
            end,
            variable_type: var_type
        }
    }
}


#[derive(Clone, Debug)]
pub struct VarReasignNode {
    pub start: Postion,
    pub end: Postion,
    pub identifer: Token,
    pub operator: Token,
    pub expr: Box<Node>,
}
impl PartialEq for VarReasignNode  {
    fn eq(&self, _other: &Self) -> bool { false }
    
}
impl VarReasignNode {
    pub fn new(identifer: Token, op: Token, expr: Node, start: Postion, end: Postion) -> Self {
        Self{
            identifer,
            operator: op,
            expr: Box::new(expr),
            start,
            end
        }
    }
}