use super::nodes::{
    number_node::NumberNode,
    string_node::StringNode,
    group_nodes::*,
    var_nodes::*,
    operation_nodes::*,
    keyword_nodes::{node_match::*, *},
    func_nodes::*,
    access_nodes::*
};
use crate::shared::{postion::Postion, token::Token};


#[derive(Clone, PartialEq, Debug)]
pub enum Node {
    EnumNode(EnumNode),
    ListNode(ListNode),
    NumberNode(NumberNode),
    ObjectNode(ObjectNode),
    StringNode(StringNode),
    DotAccessNode(DotAccessNode),
    ListAccessNode(ListAccessNode),
    PathAccessNode(PathAccessNode),
    VarAccessNode(VarAccessNode),
    VarAssignNode(VarAssignNode),
    VarReasignNode(VarReasignNode),
    BinOpNode(BinOpNode),
    UnaryOpNode(UnaryOpNode),
    BreakNode(BreakNode),
    ContinueNode(ContinueNode),
    ForNode(ForNode),
    IfNode(IfNode),
    ImportNode(ImportNode),
    NamespaceNode(NamespaceNode),
    ReturnNode(ReturnNode),
    WhileNode(WhileNode),
    FuncCallNode(FuncCallNode),
    FuncDefNode(FuncDefNode),
    MatchNode(MatchNode),
    Empty,
    MatchCatchAll
}

impl Node {
    pub fn new_enum_node(name: String, values: Vec<Token>, start: Postion, end: Postion) -> Node {
        Node::EnumNode(EnumNode::new(name, values, start, end))
    }
    pub fn new_list_node(elements: Vec<Node>, start: Postion, end: Postion) -> Node {
        Node::ListNode(ListNode::new(elements, start, end))
    }
    pub fn new_number_node(token: Token, start: Postion, end: Postion) -> Node {
        Node::NumberNode(NumberNode::new(token, start, end))
    }
    pub fn new_object_node(values: Vec<(Token,Node)>, start: Postion, end: Postion) -> Node {
        Node::ObjectNode(ObjectNode::new(values, start, end))
    }
    pub fn new_string_node(token: Token, start: Postion, end: Postion) -> Node {
        Node::StringNode(StringNode::new(token, start, end))
    }
    pub fn new_dot_access_node(path: Vec<String>, start: Postion, end: Postion) -> Node {
        Node::DotAccessNode(DotAccessNode::new(path, start, end))
    }
    pub fn new_list_access_node(variable: Token, index: Node, start: Postion, end: Postion) -> Node {
        Node::ListAccessNode(ListAccessNode::new(variable, index, start, end))
    }
    pub fn new_path_access_node(path: Vec<Token>, start: Postion, end: Postion) -> Node {
        Node::PathAccessNode(PathAccessNode::new(path, start, end))
    }
    pub fn new_var_access_node(variable: Token, start: Postion, end: Postion) -> Node {
        Node::VarAccessNode(VarAccessNode::new(variable, start, end))
    }
    pub fn new_var_assign_node(token: Token, constant: bool, node: Node, start: Postion, end: Postion, var_type: Option<Token>) -> Node {
        Node::VarAssignNode(VarAssignNode::new(token, constant, node, start, end, var_type))
    }
    pub fn new_var_reasign_node(identifer: Token, op: Token, expr: Node, start: Postion, end: Postion) -> Node {
        Node::VarReasignNode(VarReasignNode::new(identifer, op, expr, start, end))
    }
    pub fn new_bin_op_node(left_node: Node, op: Token, right_node: Node, start: Postion, end: Postion) -> Node {
        Node::BinOpNode(BinOpNode::new(left_node, op, right_node, start, end))
    }
    pub fn new_unary_op_node(token: Token, node: Node, start: Postion, end: Postion) -> Node {
        Node::UnaryOpNode(UnaryOpNode::new(token, node, start, end))
    }
    pub fn new_break_node(start: Postion, end: Postion) -> Node {
        Node::BreakNode(BreakNode::new(start, end))
    }
    pub fn new_continue_node(start: Postion, end: Postion) -> Node {
        Node::ContinueNode(ContinueNode::new(start, end))
    }
    pub fn new_for_node(var_name: Token, start_value: Node, end_value: Node, step: Node, body: Node, start: Postion, end: Postion) -> Node {
        Node::ForNode(ForNode::new(var_name, start_value, end_value, step, body, start, end))
    }
    pub fn new_if_node(statements: Vec<(Box<Node>,Box<Node>)>, else_statment: Option<Box<Node>>, start: Postion, end: Postion) -> Node {
        Node::IfNode(IfNode::new(statements, else_statment, start, end))
    }
    pub fn new_import_node(path: String, start: Postion, end: Postion) -> Node {
        Node::ImportNode(ImportNode::new(path, start, end))
    }
    pub fn new_namespace_node(name: Token, statements: Node, start: Postion, end: Postion) -> Node {
        Node::NamespaceNode(NamespaceNode::new(name, statements, start, end))
    }
    pub fn new_return_node(value: Node, start: Postion, end: Postion) -> Node {
        Node::ReturnNode(ReturnNode::new(value, start, end))
    }
    pub fn new_while_node(condition: Node, body: Node, start: Postion, end: Postion) -> Node {
        Node::WhileNode(WhileNode::new(condition, body, start, end))
    }
    pub fn new_func_call_node(call_node: Node, arguments: Vec<Box<Node>>, start: Postion, end: Postion) -> Node {
        Node::FuncCallNode(FuncCallNode::new(call_node, arguments, start, end))
    }
    pub fn new_func_def_node(name: Option<Token>, arguments: Vec<Token>, body: Node, start: Postion, end: Postion) -> Node {
        Node::FuncDefNode(FuncDefNode::new(name, arguments, body, start, end))
    }
    pub fn new_match_mode(expr: Node, matches: Vec<MatchExpr>, start: Postion, end: Postion) -> Node {
        Node::MatchNode(MatchNode::new(expr, matches, start, end))
    }
}
