//! Interperits the nodes generated form the parser
//!
//! Handles the runing of the code for the generated nodes and tokens
use crate::parser::{
    Node,
    nodes::{
        number_node::NumberNode,
        string_node::StringNode,
        group_nodes::*,
        var_nodes::*,
        operation_nodes::*,
        keyword_nodes::{node_match::*, *},
        func_nodes::*,
        access_nodes::*
    }, 
    Parser };
use crate::lexer::Lexer;
use crate::shared::{context::Context};
use crate::errors::{error::Error, RuntimeError};
use crate::types::{Varible, traits::Functional };
use crate::shared::{token_type::TokenType, postion::Postion, scope::Scope, result::IResult};

use std::cell::RefCell;
use std::rc::Rc;
use std::fs::read_to_string;

pub struct Interpreter {}

impl Interpreter {
    pub fn new() -> Self {
        Self {}
    }
    fn new_runtime_error<T>(&self,details: T, ctx: Rc<Context>,  start: Postion, end: Postion) -> Box<dyn Error> where T: ToString {
        Box::new(RuntimeError::new(details.to_string(), Some(ctx.clone()), start, end))
    }
    pub fn visit(&self, node: Node, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match node {
            Node::NumberNode(value) => {
                self.visit_number_node(value, ctx)
            }
            Node::ObjectNode(value) => {
                self.visit_object_node(value,ctx)
            }
            Node::StringNode(value) => {
                self.visit_string_node(value, ctx)
            }
            Node::VarAccessNode(value) => {
                self.visit_var_access_node(value,ctx)
            }
            Node::VarAssignNode(value) => {
                self.visit_var_assign_node(value, ctx)
            }
            Node::VarReasignNode(value) => {
                self.visit_var_reassign_node(value, ctx)
            }
            Node::PathAccessNode(value) => {
                self.visit_path_access_node(value, ctx)
            }
            Node::UnaryOpNode(value) => {
                self.visit_unary_op_node(value,ctx)
            }
            Node::BinOpNode(value) => {
                self.visit_bin_op_node(value,ctx)
            }
            Node::BreakNode(_) => {
                IResult::Ok(Varible::Break)
            }
            Node::ReturnNode(value) => {
                self.visit_return_node(value, ctx)
            }
            Node::ContinueNode(_) => {
                IResult::Ok(Varible::Continue)
            }
            Node::Empty => { unreachable!() }
            Node::MatchCatchAll => { unreachable!() }
            Node::DotAccessNode(value) => {
                self.visit_dot_access_node(value, ctx)
            }
            Node::EnumNode(value) => {
                self.visit_enum_def(value, ctx)
            }
            Node::ForNode(value) => {
                self.visit_for_node(value, ctx)
            }
            Node::FuncCallNode(value) => {
                self.visit_func_call(value, ctx)
            }
            Node::FuncDefNode(value) => {
                self.visit_func_def(value, ctx)
            }
            Node::IfNode(value) => {
                self.visit_if_node(value,ctx)
            }
            Node::WhileNode(value) => {
                self.visit_while_node(value,ctx)
            }
            Node::ImportNode(value) => {
                self.visit_import_node(value,ctx)
            }
            Node::ListAccessNode(value) => {
                self.visit_list_access_node(value,ctx)
            }
            Node::ListNode(value) => {
                self.visit_list_node(value, ctx)
            }
            Node::NamespaceNode(value) => {
                self.visit_namespace(value,ctx)
            }
            Node::MatchNode(value) => {
                self.visit_match_node(value, ctx)
            }
        }
    }
    fn visit_list_node(&self, node: ListNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        let mut elements: Vec<Varible> = vec![];

        for el in node.elements {
            match el {
                Node::Empty => {}
                _ => {
                    match self.visit(el, ctx.clone()) {
                        IResult::Err(err) => {
                            return IResult::Err(err);
                        }
                        IResult::Inter(value) => {
                            return IResult::Inter(value);
                        },
                        IResult::Ok(value) => {
                            elements.push(value);
                        }
                    }
                }
            }
        }

       IResult::Ok(Varible::List(elements))

    }
    fn visit_number_node(&self, node: NumberNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match node.token.token {
            TokenType::INT => {
                if let Some(value) = node.token.value {
                    if let Ok(e) = value.parse::<isize>() {
                        IResult::Ok(Varible::new_integer(e))
                    }else{
                        IResult::Err(self.new_runtime_error("Failed to parse integer", ctx, node.start, node.end))
                    }
                    
                } else{
                    IResult::Err(self.new_runtime_error("Failed to parse integer", ctx, node.start, node.end))
                }
            }
            TokenType::FLOAT => {
                if let Some(value) = node.token.value {
                    if let Ok(e) = value.parse::<f32>() {
                        IResult::Ok(Varible::Float(e))
                    }else{
                        IResult::Err(self.new_runtime_error("Failed to parse float", ctx, node.start, node.end))
                    }
                    
                } else{
                    IResult::Err(self.new_runtime_error("Failed to parse float", ctx, node.start, node.end))
                }
            }
            _ => {
                IResult::Err(self.new_runtime_error("Invaild number", ctx, node.start, node.end))
            }
        }
    }
    fn visit_string_node(&self, node: StringNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match node.token.value {
            Some(value) => {
                IResult::Ok(Varible::String(value))
            }
            None => {
                IResult::Err(self.new_runtime_error("Cant create string", ctx, node.start, node.end))
            }
        }
    }
    fn visit_var_access_node(&self, node: VarAccessNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible>  {
        match node.variable.value {
            Some(name) => {
                ctx.as_ref().scope.borrow().get(&name, Some(ctx.clone()))
            }
            None => {
                IResult::Err(self.new_runtime_error("No identifier", ctx.clone(), node.start, node.end))
            }
        }
    }
    fn visit_var_assign_node(&self, node: VarAssignNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible>  {
        match node.token.value {
            Some(var_name) => {
                match self.visit(*node.node, ctx.clone()) {
                    IResult::Inter(value) => IResult::Inter(value),
                    IResult::Ok(value) => {

                        // TODO: implement type checking EX: 
                        // let a: int = 1;
                        // throw a runtime if value is not a int

                       match ctx.as_ref().scope.borrow_mut().insert(&var_name,value,node.constant,Some(ctx.clone())) {
                        IResult::Err(err) => IResult::Err(err),
                        IResult::Inter(value) => IResult::Inter(value),
                        IResult::Ok(_) => IResult::Ok(Varible::Null)
                       }
                    }
                    IResult::Err(err)=>{
                        IResult::Err(err)
                    }
                }
            }
            None => {
                IResult::Err(self.new_runtime_error("Invaild identifier", ctx.clone(), node.start, node.end))
            }
        }
    }
    fn visit_var_reassign_node(&self, node: VarReasignNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible>  {
        let mut scope = ctx.as_ref().scope.borrow_mut();
        match node.identifer.value {
            Some(var_name) => {
                match self.visit(*node.expr, ctx.clone()) {
                    IResult::Inter(value) => IResult::Inter(value),
                    IResult::Ok(expr) => {
                        match node.operator.token {
                            TokenType::EQ => {
                                scope.set(&var_name,expr,Some(ctx.clone()))
                            }
                            TokenType::PLUSEQ => {
                                match scope.get(&var_name,Some(ctx.clone())) {
                                    IResult::Err(err) => IResult::Err(err),
                                    IResult::Inter(value) => IResult::Inter(value),
                                    IResult::Ok(value)=>{
                                        match value.add(&expr) {
                                            IResult::Ok(l) => scope.set(&var_name, l, Some(ctx.clone())),
                                            IResult::Err(error) => IResult::Err(error),
                                            IResult::Inter(value) => IResult::Inter(value)
                                        }
                                       
                                    }
                                }
                            }
                            TokenType::MINUSEQ => {
                                match scope.get(&var_name,Some(ctx.clone())) {
                                    IResult::Err(err) => IResult::Err(err),
                                    IResult::Inter(value) => IResult::Inter(value),
                                    IResult::Ok(value)=>{
                                        match value.sub(&expr) {
                                            IResult::Ok(l) => scope.set(&var_name, l, Some(ctx.clone())),
                                            IResult::Err(error) => IResult::Err(error),
                                            IResult::Inter(value) => IResult::Inter(value)
                                        }
                                       
                                    }
                                }
                            }
                            TokenType::DIVEQ => {
                                match scope.get(&var_name,Some(ctx.clone())) {
                                    IResult::Err(err) => IResult::Err(err),
                                    IResult::Inter(value) => IResult::Inter(value),
                                    IResult::Ok(value)=>{
                                        match value.div(&expr) {
                                            IResult::Ok(l) => scope.set(&var_name, l, Some(ctx.clone())),
                                            IResult::Inter(value) => IResult::Inter(value),
                                            IResult::Err(error) =>IResult::Err(error)
                                        }
                                       
                                    }
                                }
                            }
                            TokenType::MULEQ => {
                                match scope.get(&var_name,Some(ctx.clone())) {
                                    IResult::Err(err) => IResult::Err(err),
                                    IResult::Inter(value) => IResult::Inter(value),
                                    IResult::Ok(value)=>{
                                        match value.mul(&expr) {
                                            IResult::Ok(l) => scope.set(&var_name, l, Some(ctx.clone())),
                                            IResult::Inter(value) => IResult::Inter(value),
                                            IResult::Err(error) => IResult::Err(error)
                                        }
                                       
                                    }
                                }
                            }
                            _ => IResult::Err(self.new_runtime_error("Invaild operation", ctx.clone(), node.start, node.end))
                        }
                    }
                    IResult::Err(err) => IResult::Err(err)
                }
            }
            None => IResult::Err(self.new_runtime_error("Invaild identifier", ctx.clone(), node.start, node.end))
        }
    }
    fn visit_bin_op_node(&self, node: BinOpNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let left_node = match self.visit(*node.left_node, ctx.clone()) {
            IResult::Ok(value) => value,
            IResult::Err(err) => return IResult::Err(err),
            IResult::Inter(value) => return IResult::Inter(value) 
        };
        let right_node = match self.visit(*node.right_node, ctx.clone()) {
            IResult::Ok(value) => value,
            IResult::Inter(value) => return IResult::Inter(value),
            IResult::Err(err) => return IResult::Err(err)
        };

        match node.token.token {
            TokenType::PLUS => {
                left_node.add(&right_node)
            }
            TokenType::MINUS => {
                left_node.sub(&right_node)
            }
            TokenType::MUL => {
                left_node.mul(&right_node)
            }
            TokenType::DIV => {
                left_node.div(&right_node)
            }
            TokenType::POWER => {
                left_node.power(&right_node)
            }
            TokenType::EE => {
                left_node.comp(&right_node,false)
            }
            TokenType::NE => {
                left_node.comp(&right_node,true)
            }
            TokenType::LT => {
                left_node.lt(&right_node)
            },
            TokenType::LTE => {
                left_node.lte(&right_node)
            }, 
            TokenType::GT => {
                left_node.gt(&right_node)
            },
            TokenType::GTE => {
                left_node.gte(&right_node)
            }
            TokenType::AND => {
                left_node.and(&right_node)
            },
            TokenType::OR => {
                left_node.or(&right_node)
            },
            _ =>  IResult::Err(self.new_runtime_error("Invaild operation", ctx.clone(), node.start, node.end))
        }


    }
    fn visit_unary_op_node(&self, node: UnaryOpNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self.visit(*node.node, ctx.clone()) {
            IResult::Err(err) => IResult::Err(err),
            IResult::Inter(value) => IResult::Inter(value),
            IResult::Ok(value) => {
                match value {
                    Varible::Float(v) => {
                        match node.token.token {
                            TokenType::NOT => {
                                IResult::Ok(Varible::new_bool(!(v != 0.0)))
                            }
                            TokenType::MINUS => {
                                IResult::Ok(Varible::Float(v * -1.0))
                            }
                            _ => {
                                IResult::Ok(value)
                            }
                        }
                    }
                    Varible::Integer(v) => {
                        match node.token.token {
                            TokenType::NOT => {
                                IResult::Ok(Varible::new_bool(!(v.value != 0)))
                            }
                            TokenType::MINUS => {
                                IResult::Ok(Varible::new_integer(v.value * -1))
                            }
                            _ => {
                                IResult::Ok(Varible::Integer(v))
                            }
                        }
                    }
                    _ => {
                        IResult::Ok(value)
                    }
                }
            }
        }
    }
    fn visit_object_node(&self, node: ObjectNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let mut props: Vec<(String,Varible)> = vec![];

        for prop in node.values {
            match self.visit(prop.1, ctx.clone()) {
                IResult::Ok(node_value) => {
                    match prop.0.value {
                        Some(name) => {
                            props.push((name,node_value));
                        }
                        None => return IResult::Err(self.new_runtime_error("Object creation error", ctx.clone(), node.start, node.end))
                    }
                    
                },
                IResult::Err(err) => return IResult::Err(err),
                IResult::Inter(value) => return IResult::Inter(value) 
            }
        }

        IResult::Ok(Varible::new_object(&props))

    }
    fn visit_enum_def(&self, node: EnumNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        let mut keys: Vec<String> = vec![];
        for key in node.keys {
            match key.value {
                Some(value) => {
                    keys.push(value)
                }
                None => {}
            }
        }
        match ctx.as_ref().scope.borrow_mut().insert(&node.name,Varible::new_enum(node.name.clone(),keys),false,Some(ctx.clone())) {
            IResult::Ok(value) => IResult::Ok(value),
            IResult::Err(err) =>  IResult::Err(err) ,
            IResult::Inter(value) => IResult::Inter(value) 
        }
    }
    fn visit_namespace(&self, node: NamespaceNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let mut scope = ctx.as_ref().scope.borrow_mut();
        if scope.has_key(&node.name) {
            match scope.get(&node.name,Some(ctx.clone())) {
                IResult::Ok(value) => {
                    if let Varible::Namespace(ns) = value {
                        match self.visit(*node.statements, ns.context.clone()) {
                            IResult::Err(err) => return IResult::Err(err),
                            IResult::Ok(_) => {}
                            IResult::Inter(_) => {}
                        }
                        IResult::Ok(Varible::Namespace(ns))
                    }else {
                        IResult::Err(self.new_runtime_error("Object creation error", ctx.clone(), node.start, node.end))
                    }
                    
                }
                IResult::Err(err) => IResult::Err(err),
                IResult::Inter(value) => return IResult::Inter(value) 
            }
        } else {

            let ns = Varible::new_namespace(node.name.clone(), ctx.clone(), node.start);

            match self.visit(*node.statements, ns.context.clone()) {
                IResult::Err(err) => return IResult::Err(err),
                IResult::Ok(_) => {}
                IResult::Inter(_) => {}
            }

            scope.insert(&node.name,Varible::Namespace(ns),true,Some(ctx.clone()))
        }
     
    }
    fn visit_func_def(&self, node: FuncDefNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        let name = if let Some(value) = node.function_name {
            value.value.unwrap()
        } else {
            String::from("<anoymous>")
        };

        let mut arg_names: Vec<String> = vec![];

        for arg in node.arguments {
           arg_names.push(arg.value.unwrap())
        }

        if name != String::from("<anoymous>") {
            ctx.as_ref().scope.borrow_mut().insert(&name,Varible::new_function(name.clone(), arg_names, *node.body, node.start, node.end),false,Some(ctx.clone()))
        }else {
            IResult::Ok(Varible::new_function(name, arg_names, *node.body, node.start, node.end))
        }
    }
    fn visit_func_call(&self, node: FuncCallNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let mut args: Vec<Varible> = vec![];

        for argument in node.arguments {
            match self.visit(*argument, ctx.clone()) {
                IResult::Ok(ar) => {
                    args.push(ar);
                }
                IResult::Err(err)=> return IResult::Err(err),
                IResult::Inter(_) => return IResult::Err(self.new_runtime_error("Invaild argument", ctx.clone(), node.start, node.end))               
            }
        }

        match self.visit(*node.call, ctx.clone()) {
            IResult::Err(err) => IResult::Err(err),
            IResult::Ok(call) => {
                match call {
                    Varible::Function(func) => {
                        // TODO: Implement return statments
                        match func.exceute(args, ctx.clone()) {
                            IResult::Err(err) => IResult::Err(err),
                            IResult::Ok(value) => {
                                match value {
                                    Varible::Return(returned) => IResult::Ok(*returned),
                                    _ => {
                                        //println!("{:#?}",value);
                                        IResult::Ok(Varible::Null)
                                    }
                                }
                            }
                            IResult::Inter(value) => {
                                match value {
                                    Varible::Return(returned) => {
                                        IResult::Ok(*returned)
                                    }
                                    _ => {
                                        IResult::Ok(Varible::Null)
                                    }
                                }
                            }
                        }
                    }
                    Varible::BuiltIn(builtin) => {
                        match builtin.exceute(args, ctx.clone()) {
                            IResult::Err(err) => IResult::Err(err),
                            IResult::Ok(value) => {
                                match value {
                                    _ => IResult::Ok(Varible::Null)
                                }
                            }
                            IResult::Inter(value) => IResult::Ok(value) 
                        }
                    }
                    _ => {
                        IResult::Err(self.new_runtime_error("Expected Function call", ctx.clone(), node.start, node.end))
                    }
                }
            } 
            IResult::Inter(value) => IResult::Ok(value) 
        }


    }
    fn visit_list_access_node(&self, node: ListAccessNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        match self.visit(*node.index,ctx.clone()) {
            IResult::Ok(index) => {
                match index {
                    Varible::Integer(int) => {
                        let list = ctx.as_ref().scope.borrow().get(&node.variable_name,Some(ctx.clone()));
                        match list {
                            IResult::Ok(list_value) => {
                                if let Varible::List(list_v) = list_value {
                                    IResult::Ok(list_v[int.value as usize].clone())
                                }else{
                                    IResult::Err(self.new_runtime_error("expected list", ctx.clone(), node.start, node.end))
                                }
                            }
                            IResult::Err(err) => IResult::Err(err),
                            IResult::Inter(value) => return IResult::Inter(value) 
                        }
                       
                    }
                    _ => IResult::Err(self.new_runtime_error("Can't index list with given expr", ctx.clone(), node.start, node.end))
                }
               
            }
            IResult::Err(err) => IResult::Err(err),
            IResult::Inter(value) => IResult::Inter(value) 
        }
    }
    fn path_resulton(&self, ctx: Rc<Context>, path: &Vec<String>, index: usize) -> IResult<Varible, Box<dyn Error>, Varible> {
        
       match ctx.as_ref().scope.borrow().get(&path[index],Some(ctx.clone())) {
            IResult::Ok(value) => {
                match value {
                    Varible::Enum(value) => {
                        match &path.get(index + 1) {
                            Some(path_value) => {
                                value.get(path_value)
                            }
                            None => {
                                IResult::Err(self.new_runtime_error("Out of range", ctx.clone(), Postion::default(), Postion::default()))
                            }
                        }  
                    }
                    Varible::Namespace(value) => {
                        self.path_resulton(value.context, path, index + 1)   
                    }
                    a => IResult::Ok(a)
                }
            }
            IResult::Err(err) => IResult::Err(err),
            IResult::Inter(value) => IResult::Inter(value) 
        }
    }
    fn visit_path_access_node(&self, node: PathAccessNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        
        if node.path.is_empty() {
            return IResult::Ok(Varible::Null);
        }

        self.path_resulton(ctx, &node.path, 0)

    }
    fn visit_return_node(&self, node: ReturnNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        match self.visit(*node.value,ctx) {
            IResult::Err(err) => IResult::Err(err),
            IResult::Ok(value) => IResult::Inter(Varible::Return(Box::new(value))),
            IResult::Inter(value) => IResult::Inter(value) 
        }
    }
    fn dot_path_resultion(&self, object: Varible, ctx: Rc<Context>, path: &Vec<String>, index: usize) -> IResult<Varible, Box<dyn Error>, Varible> {

       match object {
           Varible::Object(value) => { // OBJECT 
               match path.get(index) {
                   Some(first_index) => { // INDEX 
                       match value.get(first_index) {
                           Err(err) => IResult::Err(err), 
                           Ok(obj_value) => { // OBJECT
                               match obj_value {
                                   Varible::Object(_) => {
                                        self.dot_path_resultion(obj_value, ctx.clone(), path, index + 1)
                                   } 
                                   a => IResult::Ok(a)
                               }
                           }
                       }
                   }
                   None => IResult::Err(self.new_runtime_error("Out of range", ctx.clone(), Postion::default(), Postion::default()))
               }
           }
           object => IResult::Ok(object)
       }

        
    }
    fn visit_dot_access_node(&self, node: DotAccessNode, ctx: Rc<Context>) ->  IResult<Varible, Box<dyn Error>, Varible> {
        
        let path = node.path;
        match ctx.as_ref().scope.borrow().get(&path[0],Some(ctx.clone())) {
            IResult::Ok(value) => {
                self.dot_path_resultion(value, ctx.clone(), &path, 1)
            }
            IResult::Err(err) => IResult::Err(err),
            IResult::Inter(value) => IResult::Inter(value) 
        }
    }
    fn visit_if_node(&self, node: IfNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        for statment in node.statements {
           match self.visit(*statment.0, ctx.clone()) {
               IResult::Err(err) => return IResult::Err(err),
               IResult::Ok(condition) => {
                   if condition.is_true() {
                    let mut scope = Scope::new();
                    match scope.set_parent(ctx.as_ref().scope.clone()) {
                        Err(error) => return IResult::Err(error),
                        Ok(_) => {}
                    }
                    let if_ctx = Context::new("<If>", &ctx.file_name.clone(), Rc::new(RefCell::new(scope)), Some(ctx.clone()), Some(node.start));
                    return self.visit(*statment.1, Rc::new(if_ctx));
                   }
               }
               IResult::Inter(value) => return IResult::Inter(value) 
           }
        }

        match node.else_statment {
            Some(value) => {
                let mut scope = Scope::new();
                match scope.set_parent(ctx.as_ref().scope.clone()) {
                    Err(error) => return  IResult::Err(error),
                    Ok(_) => {}
                }
                let if_ctx = Context::new("<else>", &ctx.file_name.clone(), Rc::new(RefCell::new(scope)), Some(ctx.clone()), Some(node.start));

                return self.visit(*value, Rc::new(if_ctx));
            }
            None => {}
        }

        IResult::Ok(Varible::Null)
    }
    fn visit_for_node(&self, node: ForNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let start_value = match self.visit(*node.start_value,ctx.clone()) {
            IResult::Err(err) => return IResult::Err(err),
            IResult::Inter(_) => return IResult::Err(self.new_runtime_error("Invaild for start value", ctx.clone(), node.start, node.end)),
            IResult::Ok(value) => {
                match value {
                    Varible::Integer(start) => start.value,
                    _ => return IResult::Err(self.new_runtime_error("Expected a integer", ctx.clone(), node.start, node.end)),
                }
            }
        };
        let end_value = match self.visit(*node.end_value,ctx.clone()) {
            IResult::Err(err) => return IResult::Err(err),
            IResult::Inter(_) => return IResult::Err(self.new_runtime_error("Invaild for start value", ctx.clone(), node.start, node.end)),
            IResult::Ok(value) => {
                match value {
                    Varible::Integer(end) => end.value,
                    _ => return IResult::Err(self.new_runtime_error("Expected a integer", ctx.clone(), node.start, node.end)),
                }
            }
        };

        let step_value = match self.visit(*node.step,ctx.clone()) {
            IResult::Err(err) => return IResult::Err(err),
            IResult::Inter(_) => return IResult::Err(self.new_runtime_error("Invaild for start value", ctx.clone(), node.start, node.end)),
            IResult::Ok(value) => {
                match value {
                    Varible::Integer(end) => end.value,
                    _ => return IResult::Err(self.new_runtime_error("Expected a integer", ctx.clone(), node.start, node.end)),
                }
            }
        };

        let var_name = match node.var_name.value {
            Some(value) => value,
            None => return IResult::Err(self.new_runtime_error("Expected a idefinfer", ctx.clone(), node.start, node.end)),
        };


        let mut scope = Scope::new();

        match scope.set_parent(ctx.as_ref().scope.clone()) {
            Ok(_) => {}
            Err(err) => return IResult::Err(err)
        }

       match scope.insert(&var_name, Varible::new_integer(start_value), true, Some(ctx.clone())) {
           IResult::Err(e) => return IResult::Err(e),
           _ => {}
       }

       let context = Rc::new(Context::new("<for>", &ctx.file_name.clone(), Rc::new(RefCell::new(scope)), Some(ctx.clone()), Some(node.start)));
       
       let mut i = start_value;
       let mut inter = context.as_ref().scope.borrow_mut();
       if step_value >= 0 {
        while i < end_value {
            inter.set(&var_name,Varible::new_integer(i),Some(context.clone()));
            i += step_value;

            match self.visit(*node.body.clone(),context.clone()) {
                IResult::Ok(_) => {}
                IResult::Err(err) => return  IResult::Err(err),
                IResult::Inter(value) => {
                    match value {
                        Varible::Break => {
                            break;
                        }
                        Varible::Continue => {
                            continue;
                        }
                        Varible::Return(_) => { return IResult::Inter(value); }
                        _ => {}
                    }
                }
            }
        }
       }else {
            while i > end_value {
                inter.set(&var_name,Varible::new_integer(i),Some(context.clone()));
                i += step_value;

                match self.visit(*node.body.clone(),context.clone()) {
                    IResult::Ok(_) => {}
                    IResult::Err(err) => return  IResult::Err(err),
                    IResult::Inter(value) => {
                        match value {
                            Varible::Break => {
                                break;
                            }
                            Varible::Continue => {
                                continue;
                            }
                            Varible::Return(_) => { return IResult::Inter(value); }
                            _ => {}
                        }
                    }
                }
        }
       }

        IResult::Ok(Varible::Null)
    }
    fn visit_while_node(&self, node: WhileNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {
        
        let mut scope = Scope::new();

        match scope.set_parent(ctx.as_ref().scope.clone()) {
            Ok(_) => {}
            Err(err) => return IResult::Err(err)
        }
        let context = Rc::new(Context::new("<while>", &ctx.file_name.clone(), Rc::new(RefCell::new(scope)), Some(ctx.clone()), Some(node.start)));


        loop {
            match self.visit(*node.condition.clone(),ctx.clone()) {
                IResult::Ok(value) => {
                    if !value.is_true() {
                        break;
                    }

                    match self.visit(*node.body.clone(), context.clone()) {
                        IResult::Err(error) => {
                            return IResult::Err(error);
                        }
                        IResult::Ok(_) => {}
                        IResult::Inter(inter) => {
                            match inter {
                                Varible::Break => {
                                    break;
                                }
                                Varible::Continue => {
                                    continue;
                                }
                                Varible::Return(_) => { return IResult::Inter(inter);  }
                                _ => {}
                            }
                        }
                    }
                }
                IResult::Err(error) => {
                    return IResult::Err(error);
                }
                IResult::Inter(_) => {
                    return IResult::Err(self.new_runtime_error("Invaild condition", ctx.clone(), node.start, node.end));
                }
            }

        }


        IResult::Ok(Varible::Null)
        
    }
    fn visit_match_node(&self, node: MatchNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        match self.visit(*node.expr, ctx.clone()) {
            IResult::Ok(value) => {
                let mut index = 0;
                while index < node.matches.len() {
                    let indexed = match node.matches.get(index) {
                        Some(value) => value,
                        None => return IResult::Err(self.new_runtime_error("Out of range", ctx.clone(), node.start, node.end))
                    };

                    let mut scope = Scope::new();
                    match scope.set_parent(ctx.as_ref().scope.clone()) {
                        Err(error) => return IResult::Err(error),
                        _ => {}
                    }

                    let context = Rc::new(Context::new("<match arm>", &ctx.file_name.clone(), Rc::new(RefCell::new(scope)), Some(ctx.clone()), Some(node.start)));

                    let scoped = indexed.scoped;

                    if *indexed.check == Node::MatchCatchAll {
                        match self.visit(*indexed.expr.clone(), if scoped { context.clone() } else { ctx.clone() }) {
                            IResult::Ok(value) => {
                                if !scoped {
                                    return IResult::Ok(value)
                                }
                                return IResult::Ok(Varible::Null);
                            }
                            IResult::Err(err) => return IResult::Err(err),
                            IResult::Inter(value) => {
                                if scoped {
                                    match value {
                                        Varible::Return(returned) => {
                                            return IResult::Ok(*returned)
                                        }
                                        Varible::Break | Varible::Continue => {
                                            return IResult::Inter(value)
                                        }
                                        _ => {
                                            return IResult::Err(self.new_runtime_error("Invaild interupt", ctx.clone(), node.start, node.end));
                                        }
                                    }
                                }
                                return IResult::Inter(value);
                            }
                        }
                    }

                    match self.visit(*indexed.check.clone(), ctx.clone()) {
                        IResult::Ok(check) => {
                            if value == check {
                                match self.visit(*indexed.expr.clone(), if scoped { context.clone() } else { ctx.clone() }) {
                                    IResult::Ok(value) => {
                                        if !scoped {
                                            return IResult::Ok(value)
                                        }
                                        return IResult::Ok(Varible::Null);
                                    }
                                    IResult::Err(err) => return IResult::Err(err),
                                    IResult::Inter(value) => {
                                        if scoped {
                                            match value {
                                                Varible::Return(returned) => {
                                                    return IResult::Ok(*returned)
                                                }
                                                Varible::Break | Varible::Continue => {
                                                    return IResult::Inter(value)
                                                }
                                                _ => {
                                                    return IResult::Err(self.new_runtime_error("Invaild interupt", ctx.clone(), node.start, node.end));
                                                }
                                            }
                                        }
                                        return IResult::Inter(value);
                                    }
                                }
                            }
                        }
                        IResult::Err(err) => return IResult::Err(err),
                        IResult::Inter(_) => {
                            return IResult::Err(self.new_runtime_error("Invaild match check value", ctx.clone(), node.start, node.end))
                        }
                    } 
                    index += 1;
                }
            }
            IResult::Err(error) => return IResult::Err(error),
            IResult::Inter(_) => return IResult::Err(self.new_runtime_error("Invaild expresion", ctx.clone(), node.start, node.end))
        }



        IResult::Ok(Varible::Null)
    }
    fn visit_import_node(&self, node: ImportNode, ctx: Rc<Context>) -> IResult<Varible, Box<dyn Error>, Varible> {

        let path = std::path::Path::new(&node.path);

        if !path.is_file() {
            return IResult::Err(self.new_runtime_error("Import is not a file", ctx.clone(), node.start, node.end));
        } 

        match read_to_string(path) {
            Err(err) => {
                println!("{:#?}",err);
                IResult::Err(self.new_runtime_error("Failed to load module", ctx.clone(), node.start, node.end))
            }
            Ok(contents) => {
                let mut lexer = Lexer::new(contents, String::from(path.file_name().expect("Import was not file").to_str().expect("Invaild UTF-8 path")));
                match lexer.make_tokens() {
                    Ok(tokens) => {
                        match Parser::new(tokens.0).parse() {
                            Ok(node) => {
                                self.visit(node,ctx.clone())
                            }
                            Err(err) => {
                                IResult::Err(err)
                            }
                        }
                    }
                    Err(err) => return IResult::Err(err)
                }
            }
        }


    }
}