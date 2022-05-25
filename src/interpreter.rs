use crate::{
    shared::{
        Node, 
        Position, 
        TokenType, 
        Token
    }, 
    errors::RuntimeError
};
use std::{
    sync::{
        Arc, 
        Mutex
    }, 
    fmt::{Display, Debug}, 
    path::PathBuf, 
    collections::{ 
        HashMap, 
        HashSet 
    }, 
    borrow::{
        BorrowMut, 
        Borrow
    }, ops::Add, fs
};
use dyn_clone::DynClone;
use colored::*;
use async_recursion::async_recursion;
use async_trait::async_trait;
use crate::standard_lib::{
    io::insert_io,
    http::insert_http,
    fs::insert_fs
};
use crate::runner;

#[async_trait]
pub trait Functional: Debug + DynClone + Send + Sync {
    async fn execute(&self, args: Vec<DataType>) -> VisitResult<DataType>;
}
dyn_clone::clone_trait_object!(Functional);

#[derive(Debug)]
pub enum VisitResult<T> {
    Ok(T),
    Error(RuntimeError),
    Break(T)
}


#[derive(Debug,Clone)]
pub enum DataType {
    BOOL {
        value: bool
    },
    STRING {
        value: String
    },
    INTEGER {
        value: isize
    },
    OBJECT {
        value: HashMap<String,DataType>
    },
    LIST {
        value: Vec<DataType>
    },
    FUNCTION {
        name: String,
        body: Node,
        args: Vec<String>   
    },
    NativeFunction {
        name: String,
        args: Vec<String>,
        method: Box<dyn Functional>
    },
    NAMESPACE(Ctx),
    NULL
}

impl DataType {
    fn int(value: isize) -> Self {
        DataType::INTEGER { value }
    }
    fn string(value: String) -> Self {
        DataType::STRING { value }
    }
    fn list(value: Vec<DataType>) -> Self {
        DataType::LIST { 
            value 
        }
    }
    fn boolean(value: bool) -> Self {
        DataType::BOOL { value }
    }
    fn object(value: HashMap<String,DataType>) -> Self {
        Self::OBJECT { value }
    }
    fn function(name: String, args: Vec<String>, body: Node) -> Self {
        Self::FUNCTION { name, args, body }
    }

    fn add(&self,right: &DataType) -> VisitResult<DataType> {
        match self { 
            DataType::INTEGER { value: self_value } => {
                let right_value = match right { 
                    DataType::INTEGER { value } => value,
                    _ => return VisitResult::Error(RuntimeError::OperationError { 
                        op: "add"
                    })
                };

                VisitResult::Ok(DataType::INTEGER { 
                    value: self_value + right_value  
                })
            },
            DataType::STRING { value: self_value } => {
                let right_value = match right { 
                    DataType::STRING { value } => value,
                    _ => return VisitResult::Error(RuntimeError::OperationError { 
                        op: "add"
                    })
                };

                VisitResult::Ok(DataType::STRING { 
                    value: self_value.clone().add(right_value.as_str())
                })
            },
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "add"
            })
        }
    }
    fn div(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "div"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "div"
            })
        };

        VisitResult::Ok(DataType::INTEGER { 
            value: self_value / right_value  
        })
    }
    fn sub(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "minus"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "minus"
            })
        };

        VisitResult::Ok(DataType::INTEGER { 
            value: self_value - right_value  
        })
    }
    fn mul(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "div"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "div"
            })
        };

        VisitResult::Ok(DataType::INTEGER { 
            value: self_value * right_value  
        })
    }
    fn is_true(&self) -> bool {
        match self {
            DataType::BOOL { value } => *value,
            DataType::STRING { value } => value.len() > 0,
            DataType::INTEGER { value } => *value != 0,
            DataType::NULL => false,
            _ => true
        }
    }
    fn gt(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "gt"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "gt"
            })
        };

        VisitResult::Ok(DataType::boolean(self_value > right_value ))
    }
    fn lt(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "lt"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "lt"
            })
        };

        VisitResult::Ok(DataType::boolean(self_value < right_value ))
    }
    fn gte(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "gte"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "gte"
            })
        };

        VisitResult::Ok(DataType::boolean(self_value >= right_value ))
    }
    fn lte(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "lte"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "lte"
            })
        };

        VisitResult::Ok(DataType::boolean(self_value <= right_value ))
    }
    fn eq(&self, right: &DataType) -> VisitResult<DataType> {
        let self_value = match self { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "eq"
            })
        };
        let right_value = match right { 
            DataType::INTEGER { value } => value,
            _ => return VisitResult::Error(RuntimeError::OperationError { 
                op: "eq"
            })
        };

        VisitResult::Ok(DataType::boolean(self_value == right_value))
    }
}

impl Display for DataType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::STRING { value } => write!(f,"{}",format!(r#""{}""#,value).green()),
            Self::INTEGER { value} => write!(f,"{}",format!("{}",value).blue()),
            Self::NULL => write!(f,"{}","NULL".bright_black()),
            Self::NativeFunction { name, args: _args, method: _ } => write!(f,"<NativeFunction {}>",name),
            Self::LIST { value } => {
                let elems = value.iter().map(|d| format!("{}",d)).collect::<Vec<String>>().join(",");
                write!(f,"[{}]",elems)
            },
            Self::BOOL {value} => write!(f,"{}",format!("{}",value).red()),
            Self::OBJECT { value } => {
                let props = value.iter().map(|(a,b)| format!("{}: {}",a,b)).collect::<Vec<String>>().join(",");
                write!(f,"{{ {} }}",props)
            },
            Self::FUNCTION {name,args: _b, body: _a} => write!(f,"{} {}{}","<function".bright_black(), name.bright_cyan(),">".bright_black()),
            Self::NAMESPACE( ctx ) => {
                match ctx.lock() {
                    Err(err) => write!(f,"{}","Failed to access namespace context".red()),
                    Ok(lock)=>write!(f,"{} {}{}","<namespace".bright_black(),lock.name.white(),">".bright_black())
                }
            }
           // _ => write!(f, "<MISSING>")
        }
    }
}
#[derive(Debug)]
pub struct Property {
    pub writable: bool,
    pub value: DataType
}
impl Property {
    pub fn new(value: DataType, writable: bool) -> Self {
        Self {
            value,
            writable
        }
    }
}

#[derive(Debug)]
pub struct Context {
    parent: Option<Arc<Mutex<Self>>>,
    name: String,
    file: PathBuf,
    scope: HashMap<String,Property>,
    imports: HashSet<String>
}

pub type Ctx = Arc<Mutex<Context>>;

impl Context {
    pub fn add_import(&mut self, file: String) -> anyhow::Result<bool> {
        match &self.parent {
            Some(parent) => {
                match parent.lock(){
                    Ok(mut lock) => lock.add_import(file),
                    Err(_err) => bail!("Failed to get context")
                }
            }
            None => Ok(self.imports.insert(file))
        }
    }
    pub fn new(parent: Option<Arc<Mutex<Context>>>, name: String, file: PathBuf) -> Self {
        Self {
            parent,
            name,
            file,
            scope: HashMap::new(),
            imports: HashSet::new()
        }
    }
    pub fn with(parent: Option<Arc<Mutex<Context>>>, name: String, file: PathBuf, vars: HashMap<String,Property>) -> Self {
        Self {
            parent,
            name,
            file,
            scope: vars,
            imports: HashSet::new()
        }
    }
    pub fn default(file: PathBuf) -> Self {
        let mut scope = HashMap::new();

        scope.insert("true".to_string(), Property { value: DataType::boolean(true), writable: false });
        scope.insert("false".to_string(), Property { value: DataType::boolean(false), writable: false });
        scope.insert("null".to_string(), Property { value: DataType::NULL, writable: false });

        insert_io(&mut scope);
        insert_http(&mut scope);
        insert_fs(&mut scope);

        Self {
            parent: None,
            name: "<main>".to_string(),
            file,
            imports: HashSet::new(),
            scope
        }
    }
    pub fn has_var<T>(&self, key: &T) -> bool where T: ToString {
        self.scope.contains_key(&key.to_string())
    }
    pub fn set_var<T>(&mut self, key: T, value: DataType) -> anyhow::Result<DataType> where T: ToString {
        match self.scope.get_mut(&key.to_string()) {
            None => {
                match &self.parent {
                    Some(parent) => {
                        match parent.lock() {
                            Ok(mut lock) => {
                                lock.set_var(key, value)
                            }
                            Err(err) => {
                                error!("{}",err);
                                bail!(RuntimeError::ReferenceError { 
                                    reason: format!("Permission denied to access property '{}'",key.to_string()).to_string()
                                })
                            }
                        }
                    }
                    None => bail!(RuntimeError::ReferenceError { 
                        reason: format!("assignment to undeclared variable '{}'",key.to_string()).to_string()
                    })
                }
            }
            Some(var) => {
                if var.writable {
                    var.value = value;
                    Ok(var.value.to_owned())
                } else {
                    bail!(RuntimeError::TypeError { 
                        reason:  format!("invalid assignment to const '{}'",key.to_string()).to_string()
                    })
                }
            }
        }
    }
    pub fn get<T>(&self, key: T) -> anyhow::Result<DataType> where T: ToString {
        match self.scope.get(&key.to_string()) {
            Some(prop) => {
                Ok(prop.value.to_owned())
            }
            None => {
                match &self.parent {
                    Some(parent) => {
                        match parent.lock() {
                            Ok(lock) => lock.borrow().get(key),
                            Err(err) => {
                                error!("{}",err);
                                bail!(RuntimeError::ReferenceError { 
                                    reason: format!("Permission denied to access property '{}'",key.to_string()).to_string()
                                })
                            }
                        }
                    }
                    None =>  bail!(RuntimeError::ReferenceError { 
                        reason: format!("'{}' is not defined",key.to_string()).to_string() 
                    })
                }
            }
        }
    }
    pub fn insert<T>(&mut self, key: T, value: DataType, writable: bool) -> anyhow::Result<DataType> where T: ToString {
        if self.has_var(&key) {
            bail!(RuntimeError::ReferenceError { 
                reason: format!("can't define property")
            });
        }

        self.scope.insert(key.to_string(), Property::new(value.clone(), writable));

        Ok(value)
    }
}
pub struct Interperter {}

impl Interperter {
    pub async fn run(node: Node, ctx: Ctx) -> anyhow::Result<DataType> {
        let inter = Self {};
        match inter.visit(node, ctx).await {
            VisitResult::Ok(value) => Ok(value),
            VisitResult::Break(value) => Ok(value),
            VisitResult::Error(err) => bail!(err)
        }
    }
    #[async_recursion]
    async fn visit(&self, node: Node, ctx: Ctx) -> VisitResult<DataType> {
        match node {
           Node::Empty => VisitResult::Ok(DataType::NULL),
           Node::PropAccess { start, end, path } => self.visit_prop(ctx,start,end,path).await,
           Node::Import { start, end, from, namespace } => self.visit_import(ctx, start, end, from, namespace).await,
           Node::Namespace { start, end, identifer, scope } => self.visit_namespace(ctx, start, end, identifer, scope).await,
           Node::While { start, end, scope, expr } => self.visit_while(ctx,start,end,scope,expr).await,
           Node::For { start, end, iterator, element_key, scope } => self.visit_for(ctx,start,end,iterator,element_key,scope).await,
           Node::If { start, end, expr, body } => self.visit_if(ctx,start,end,expr,body).await,
           Node::Return { start, end, node } => self.visit_return(ctx,start,end,node).await,
           Node::Function { start, end, args, body, name } => self.visit_function(ctx,start,end,args,body,name).await,
           Node::BinOperation { left, operator, right, start, end } => self.visit_bin_operation(ctx,left, operator, right, start, end).await,
           Node::FunctionCall { identifer, arguments, start, end } => self.visit_function_call(ctx,identifer, arguments, start, end).await,
           Node::Integer { start, end, value } => self.visit_integer(value, start, end).await,
           Node::List { values , start ,end} => self.visit_list(ctx,values, start, end).await,
           Node::IndexingAccess { identifier , index, start, end } => self.visit_indexing_access(ctx,identifier, index, start, end).await,
           Node::Object { props , start , end } => self.visit_object(ctx,props, start, end).await,
           Node::Scope { statements  } => self.visit_scope(ctx,statements).await,
           Node::String { start, end, value } => self.visit_string_node(value, start, end).await,
           Node::UnaryOperation { operator, value, start, end } => self.visit_unary_operation(ctx,operator, value, start, end).await,
           Node::VarableAccess { start, end, identifer } => self.visit_varable_access(ctx,identifer, start, end).await,
           Node::VarableAsignment { identifer, operation, value, start, end } => self.visit_varable_asignment(ctx,identifer, operation, value, start, end).await,
           Node::VarableDeclarement { identifer, is_const, value, start, end } => self.visit_varable_declarement(ctx, identifer, is_const, value, start, end).await,
        }
    }
    async fn visit_prop(&self, ctx: Ctx, _start: Position, _end: Position, path: Vec<String>) -> VisitResult<DataType> {
        let root = match path.get(0) {
            Some(value) => value,
            None => return VisitResult::Error(RuntimeError::TypeError { reason: "Expexted a namespace,object, or class ref".to_string() })
        };
        let access = match path.get(1) {
            Some(value) => value,
            None => return VisitResult::Error(RuntimeError::TypeError { reason: "Expexted a identifer".to_string() })
        };

        let lock = match ctx.lock() {
            Ok(value) => value,
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".to_string() 
            }) 
        };

        match lock.get(root) {
            Ok(value) => {
                match value {
                    DataType::NAMESPACE(ns) => {
                        let ns_lock = match ns.lock() {
                            Ok(value) => value,
                            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                                reason: "Failed to access context".to_string() 
                            }) 
                        };

                        match ns_lock.get(access){
                            Ok(data) => VisitResult::Ok(data),
                            Err(err) => VisitResult::Error(RuntimeError::Error(err))
                        }
                    }
                    DataType::OBJECT { value: obj } => {
                        match obj.get(access) {
                            Some(data) => VisitResult::Ok(data.to_owned()),
                            None => VisitResult::Error(RuntimeError::ReferenceError { reason: format!("'{}' does not exist on object",access) })
                        }
                    }
                    _ => VisitResult::Error(RuntimeError::SyntaxError { reason: "Can't use '.' on given type".to_string() })
                }
            }
            Err(err) => VisitResult::Error(RuntimeError::Error(err))
        }
    }
    async fn visit_while(&self, ctx: Ctx, _start: Position, _end: Position, scope: Box<Node>, expr: Box<Node>) -> VisitResult<DataType> {
        let wctx = Arc::new(Mutex::new(
            Context::new(Some(ctx.clone()),"<while>".into(),PathBuf::default())
        ));

       loop {
            let node = expr.clone();
            match self.visit(*node,ctx.clone()).await {
                VisitResult::Ok(value) => {
                    if !value.is_true() {
                        break;
                    }
                }
                VisitResult::Error(err) => return VisitResult::Error(err),
                VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { reason: "Invaild statment".into() })
            }

            let snode = scope.clone();

            match self.visit(*snode, wctx.clone()).await {
                VisitResult::Ok(_) => {}
                VisitResult::Error(err) => return VisitResult::Error(err),
                VisitResult::Break(value) => return VisitResult::Break(value)
            }
        }

        VisitResult::Ok(DataType::NULL)
    }
    async fn visit_for(&self,ctx: Ctx,_start: Position, _end: Position, iterator: Box<Node>, element_key: String, scope: Box<Node>) -> VisitResult<DataType> {
        let array = match self.visit(*iterator, ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Error(err) => return VisitResult::Error(err),
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild statement" .into()
            })
        };

        let for_ctx = Arc::new(Mutex::new(
            Context::with(Some(ctx.clone()), "<for>".into(), PathBuf::default(), HashMap::from(
                [
                    (
                        element_key.clone(), 
                        Property { writable: true, value: DataType::NULL } 
                    )
                ]
            ))
        ));

        match array {
            DataType::LIST { value } => {

                for el in value {
                    let node = scope.clone();
                    let fctx = for_ctx.clone();
                    
                    match fctx.lock() {
                        Ok(mut lock) => {
                            if let Err(err) = lock.set_var(element_key.clone(), el) {
                                error!("{}",err);
                                return VisitResult::Error(RuntimeError::InternalError { 
                                    reason: "Failed to set loop varaible" 
                                });
                            }
                        }
                        Err(err) => {
                            error!("{}",err);
                            return VisitResult::Error(RuntimeError::ReferenceError { 
                                reason: "Failed to get loop context".into() 
                            });
                        }
                    } 

                   match self.visit(*node, for_ctx.clone()).await {
                       VisitResult::Error(err) => return VisitResult::Error(err),
                       VisitResult::Ok(_) => {}
                       VisitResult::Break(value) => return VisitResult::Break(value)
                   }
                }

                VisitResult::Ok(DataType::NULL)
            }
            DataType::STRING { value } => {
                for el in value.chars() {
                    let node = scope.clone();
                    let fctx = for_ctx.clone();
                    
                    match fctx.lock() {
                        Ok(mut lock) => {
                            if let Err(err) = lock.set_var(element_key.clone(), DataType::STRING { value: el.to_string() }) {
                                error!("{}",err);
                                return VisitResult::Error(RuntimeError::InternalError { 
                                    reason: "Failed to set loop varaible" 
                                });
                            }
                        }
                        Err(err) => {
                            error!("{}",err);
                            return VisitResult::Error(RuntimeError::ReferenceError { 
                                reason: "Failed to get loop context".into() 
                            });
                        }
                    } 

                   match self.visit(*node, for_ctx.clone()).await {
                       VisitResult::Error(err) => return VisitResult::Error(err),
                       VisitResult::Ok(_) => {}
                       VisitResult::Break(value) => return VisitResult::Break(value)
                   }
                }

                VisitResult::Ok(DataType::NULL)
            }
            _ => {
                VisitResult::Error(RuntimeError::SyntaxError { 
                    reason: "Can not iterate over give type" .into()
                })
            }
        }
    }
    async fn visit_if(&self,ctx: Ctx, _start: Position, _end: Position, expr: Box<Node>, body: Box<Node>) -> VisitResult<DataType> {
        let stat = match self.visit(*expr,ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Error(err) => return VisitResult::Error(err),
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild statement" .into()
            })
        };

        if stat.is_true() {
            let if_ctx = Arc::new(Mutex::new(
                Context::new(Some(ctx.clone()),"<if>".into(),PathBuf::default())
            ));

            match self.visit(*body, if_ctx).await {
                VisitResult::Ok(_) => return VisitResult::Ok(DataType::NULL),
                VisitResult::Break(value) => return VisitResult::Break(value),
                VisitResult::Error(err) => return VisitResult::Error(err)
            }
        }

        VisitResult::Ok(DataType::NULL)
    }
    async fn visit_return(&self, ctx: Ctx, _start: Position, _end: Position, node: Box<Node>) -> VisitResult<DataType> {
        match self.visit(*node, ctx).await {
            VisitResult::Ok(value) => VisitResult::Break(value),
            VisitResult::Break(value) => VisitResult::Break(value),
            VisitResult::Error(err) => VisitResult::Error(err)
        }
    }
    async fn visit_varable_declarement(&self,ctx: Ctx, identifer: Token,is_const: bool, value: Box<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        match identifer.token {
            TokenType::IDENTIFIER(var) => {
                match self.visit(*value, ctx.clone()).await {
                    VisitResult::Ok(expr) => {
                        match ctx.lock() {
                            Ok(mut lock) => {
                                match lock.borrow_mut().insert(var, expr, !is_const) {
                                    Ok(re) => VisitResult::Ok(re),
                                    Err(err) => VisitResult::Error(RuntimeError::Error(err))
                                }
                            }
                            Err(err) => {
                                error!("{}",err);
                                VisitResult::Error(RuntimeError::ReferenceError { 
                                    reason: "Failed to insert".to_string()
                                })
                            }
                        }
                    },
                    VisitResult::Break(_) => VisitResult::Error(RuntimeError::SyntaxError { 
                        reason: "Failed to access context".into()
                    }),
                    VisitResult::Error(err) => VisitResult::Error(err)
                }
            }
            _ => VisitResult::Error(RuntimeError::TypeError { 
                reason: "Invaild type".to_string() 
            })
        }
    }
    async fn visit_varable_asignment(&self, ctx: Ctx, identifer: Token, operation: Token, value: Box<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        match identifer.token {
            TokenType::IDENTIFIER(var) => {
                match self.visit(*value,ctx.clone()).await {
                    VisitResult::Ok(node) => {
                        match operation.token {
                            TokenType::EQUAL => {
                                match ctx.lock() {
                                    Ok(mut lock) => {
                                        match lock.set_var(var, node) {
                                            Ok(re) => VisitResult::Ok(re),
                                            Err(err) => VisitResult::Error(RuntimeError::Error(err))
                                        }
                                    }
                                    Err(_) => VisitResult::Error(RuntimeError::ReferenceError { 
                                        reason: "Failed to access context".to_string() 
                                    })
                                }
                            }
                            _ => VisitResult::Error(RuntimeError::SyntaxError { 
                                reason: "Invaild operation".into() 
                            })
                        }
                    }
                    VisitResult::Error(err) => VisitResult::Error(err),
                    VisitResult::Break(_) => VisitResult::Error(RuntimeError::SyntaxError { 
                        reason: "Invaild".into()
                    })
                }
            }
            _ => VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild data type".into()
            })
        }
    }
    async fn visit_varable_access(&self,ctx: Ctx, identifier: TokenType, _start: Position, _end: Position) -> VisitResult<DataType> {
       match identifier {
           TokenType::IDENTIFIER(value) => {
               match ctx.lock() {
                    Ok(lock) => {
                        match lock.get(value) {
                            Ok(data) => VisitResult::Ok(data),
                            Err(err) => VisitResult::Error(RuntimeError::Error(err))
                        }
                    }
                    Err(_) => VisitResult::Error(RuntimeError::ReferenceError { 
                        reason: "Failed to access context".to_string() 
                    })
               }
           }
           _ => VisitResult::Error(RuntimeError::SyntaxError { 
               reason: "Invaild data type" .into()
           })
       } 

    }
    async fn visit_unary_operation(&self, ctx: Ctx, operator: Token, value: Box<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        match self.visit(*value, ctx.clone()).await {
            VisitResult::Ok(el) => {
                match el {
                   DataType::INTEGER { value: integer } => {
                        match operator.token {
                            TokenType::MINUS => VisitResult::Ok(DataType::int(integer * -1)),
                            TokenType::NOT => VisitResult::Ok(DataType::boolean(!(integer != 0))),
                            _ => VisitResult::Error(RuntimeError::SyntaxError { 
                                reason: "Invaild operation".into() 
                            })
                        }
                    }
                    _ => {
                        match operator.token {
                            TokenType::NOT => VisitResult::Ok(DataType::boolean(!el.is_true())),
                            _ => VisitResult::Error(RuntimeError::TypeError { 
                                reason: "Expected a vaild data type".into()
                            })
                        }
                    }
                }
            }
            VisitResult::Break(_) => VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild".into()
            }),
            VisitResult::Error(err) => VisitResult::Error(err)
        }
    }
    async fn visit_scope(&self, ctx: Ctx, statements: Vec<Box<Node>>) -> VisitResult<DataType> {
        let mut index = 1;
        let length = statements.len();
        for statement in statements {
           match self.visit(*statement,ctx.clone()).await {
              VisitResult::Ok(value) => {
                  if index == length {
                      return VisitResult::Ok(value);
                  }
              }
              VisitResult::Error(err) =>  return VisitResult::Error(err),
              VisitResult::Break(value) => return VisitResult::Break(value)
           }
           index += 1;
        }

        VisitResult::Ok(DataType::NULL)
    }
    async fn visit_object(&self,ctx: Ctx, props: Vec<(Token,Node)>, _start: Position, _end: Position) -> VisitResult<DataType> {
        let mut propertys: HashMap<String,DataType> = HashMap::new();

        for (key,value) in props {
            match self.visit(value, ctx.clone()).await {
                VisitResult::Ok(d) => {
                    match key.token {
                        TokenType::IDENTIFIER(idt) => {
                            propertys.insert(idt, d);
                        }
                        _ => return VisitResult::Error(RuntimeError::SyntaxError { 
                            reason: "Unexpected token".into()
                        })
                    }
                }
                VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                    reason: "Invaild keyword".into() 
                }),
                VisitResult::Error(err) => return VisitResult::Error(err)
            }
        }

        VisitResult::Ok(DataType::object(propertys))
    }
    async fn visit_indexing_access(&self,ctx: Ctx, identifier: Token, index: Box<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        let key = match identifier.token {
            TokenType::IDENTIFIER(value) => value,
            _ => return VisitResult::Error(RuntimeError::TypeError { 
                reason: "Invaild data type".into() 
            })
        };

        let iv = match self.visit(*index, ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Error(err) => return VisitResult::Error(err),
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild statement".into() 
            })
        };

        let lock = match ctx.lock() {
            Ok(value) => value,
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".into()
            })
        };

        match lock.borrow().get(key.clone()) {
            Ok(obj) => {
                match obj {
                    DataType::OBJECT { value } => {
                        match iv {
                            DataType::STRING { value: si } => {
                                match value.get(&si) {
                                    Some(obj_value) => VisitResult::Ok(obj_value.clone()),
                                    None => VisitResult::Error(RuntimeError::ReferenceError { 
                                        reason: format!("No property with name '{}' exists on '{}'",si,key)
                                    })
                                }
                            }
                            _ => VisitResult::Error(RuntimeError::TypeError { 
                                reason: "Can not index with given data type".into()
                            }) 
                        }
                    },
                    DataType::LIST { value } => {
                        match iv {
                            DataType::INTEGER { value: gi } => {
                                match value.get(gi as usize) {
                                    Some(value) => VisitResult::Ok(value.clone()),
                                    None => VisitResult::Error(RuntimeError::RangeError { 
                                        reason: "Index out of range"
                                    })
                                }
                            }
                            _ => VisitResult::Error(RuntimeError::TypeError { 
                                reason: "Can not index with given data type".into()
                            })
                        }
                    },
                    _ => VisitResult::Error(RuntimeError::TypeError { 
                        reason: "Can not index give data type".into()
                    })
                }
            }
            Err(err) => VisitResult::Error(RuntimeError::Error(err))
        }
    }
    async fn visit_list(&self, ctx: Ctx, values: Vec<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        let mut elements: Vec<DataType> = vec![];

        for el in values {
            match self.visit(el,ctx.clone()).await {
                VisitResult::Ok(value) => {
                    elements.push(value);
                }
                VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                    reason: "Invaild pram".into()
                }),
                VisitResult::Error(err) => return VisitResult::Error(err)
            }
        }

        VisitResult::Ok(DataType::list(elements))

    }
    async fn visit_integer(&self, value: TokenType, start: Position, end: Position) -> VisitResult<DataType> {
        match value {
            TokenType::INTEGER(integer) =>  VisitResult::Ok(DataType::int(integer)),
            _ =>  VisitResult::Error(RuntimeError::InterptError { 
                start, 
                end, 
                reason: "Failed to generate integer (invaild type)", 
                ctx: "<main>" 
            })
        }
    }
    async fn visit_function(&self, ctx: Ctx, _start: Position, _end: Position, args: Vec<TokenType>, body: Box<Node>, name: String) -> VisitResult<DataType> {
        let mut arg_names: Vec<String> = vec![];

        for arg in args {
            match arg {
                TokenType::IDENTIFIER(value) => {
                    arg_names.push(value)
                }
                _ => return VisitResult::Error(RuntimeError::SyntaxError { 
                    reason: "Invaild data type".into()
                })
            }
        }

        let mut lock = match ctx.lock() {
            Ok(value) => value,
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".to_string() 
            }) 
        };

        match lock.borrow_mut().insert(name.clone(), DataType::function(name.clone(), arg_names, *body), false) {
            Ok(value) => VisitResult::Ok(value),
            Err(err) => VisitResult::Error(RuntimeError::Error(err))
        }
    }
    async fn visit_function_call(&self,ctx: Ctx, identifer: Box<Node>, arguments: Vec<Node>, _start: Position, _end: Position) -> VisitResult<DataType> {
        
        let mut args: Vec<DataType> = vec![];

        for arg in arguments {
            match self.visit(arg, ctx.clone()).await {
                VisitResult::Ok(value) => {
                    args.push(value);
                }
                VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                    reason: "Invaild statment".into() 
                }),
                VisitResult::Error(err) => return VisitResult::Error(err)
            }
        }

        let idnt = match self.visit(*identifer, ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::SyntaxError { 
                reason: "Invaild statment".into() 
            }),
            VisitResult::Error(err) => return VisitResult::Error(err)
        };

        match idnt {
            DataType::FUNCTION { name, body, args: args_list } => {

                if args_list.len() != args.len() {
                    return VisitResult::Error(RuntimeError::SyntaxError { 
                        reason: format!("Expected {} arguments found {}",args_list.len(),args.len())
                    })
                }

                let mut map = HashMap::new();

                for i in 0..args_list.len() {
                    map.insert(args_list[i].to_string(), Property { writable: true, value: args[i].clone() });
                }


                // create function context
                let func_ctx = Arc::new(Mutex::new(
                    Context::with(Some(ctx.clone()),name.clone(),PathBuf::new(),map)
                ));

             
                match self.visit(body, func_ctx).await {
                    VisitResult::Ok(_) =>  VisitResult::Ok(DataType::NULL),
                    VisitResult::Break(value) => VisitResult::Ok(value),
                    VisitResult::Error(err) => VisitResult::Error(err)
                }
            }
            DataType::NativeFunction { name: _name, args: _args_list, method } => {
                method.execute(args).await
            }
            _ => VisitResult::Error(RuntimeError::TypeError { 
                reason: "Not a callable data type".into() 
            })
        }
    }
    async fn visit_bin_operation(&self, ctx: Ctx, left: Box<Node>, operator: TokenType, right: Box<Node>, start: Position, end: Position) -> VisitResult<DataType> {
        
        let left_node = match self.visit(*left, ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Error(err) => return VisitResult::Error(err),
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::InterptError { 
                start, 
                end, 
                reason: "Found a break or conintue in a invaild place", 
                ctx: "<main>"
            })
        };

        let right_node = match self.visit(*right, ctx.clone()).await {
            VisitResult::Ok(value) => value,
            VisitResult::Error(err) => return VisitResult::Error(err),
            VisitResult::Break(_) => return VisitResult::Error(RuntimeError::InterptError { 
                start, 
                end, 
                reason: "Found a break or conintue in a invaild place", 
                ctx: "<main>"
            })
        };

        match operator {
            TokenType::MINUS => left_node.sub(&right_node),
            TokenType::PLUS => left_node.add(&right_node),
            TokenType::DIV => left_node.div(&right_node),
            TokenType::MUL => left_node.mul(&right_node),
            TokenType::ARROWL => left_node.lt(&right_node),
            TokenType::ARROWR => left_node.gt(&right_node),
            TokenType::GEATERTHENEQ => left_node.gte(&right_node),
            TokenType::LESSTHENEQ => left_node.lte(&right_node),
            TokenType::EE => left_node.eq(&right_node),
            _ => VisitResult::Error(RuntimeError::InterptError { 
                start, 
                end, 
                reason: "Failed to complete operation", 
                ctx: "<main>" 
            })
        }
    }
    async fn visit_string_node(&self,value: TokenType, start: Position, end: Position) -> VisitResult<DataType> {
        match value {
            TokenType::STRING(inter_value) => VisitResult::Ok(DataType::string(inter_value)),
            _ => VisitResult::Error(RuntimeError::InterptError { 
                start, 
                end, 
                reason: "Failed to generate string (invaild type)", 
                ctx: "<main>" 
            })
        }
    }
    async fn visit_import(&self, ctx: Ctx, _start: Position, _end: Position, from: PathBuf, namespace: String) -> VisitResult<DataType> {
        
        let path = match ctx.lock() {
            Ok(lock) => {
                let mut file = lock.file.clone();
                if !file.is_dir() {
                    file.pop();
                }
                match file.join(from.clone()).canonicalize() {
                    Ok(value) => value,
                    Err(err) => return VisitResult::Error(RuntimeError::StdError{ error: err, module: "import" })
                }
            }
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".to_string() 
            }) 
        };

        // if the import does not point to a file we cant do anything
        if !path.is_file() {
            return VisitResult::Error(RuntimeError::SyntaxError { reason: "Could not file".to_string() });
        }

        // get the absulute path; this is to help not importing file multiple times
       
        let abs_str = path.to_str().expect("Failed to convert to string").to_string();

        let ns = Arc::new(
            Mutex::new(
                Context::new(Some(ctx.clone()),namespace.clone(),PathBuf::default())
            )
        );

        let file = match fs::read_to_string(path.clone()) {
            Ok(value) => value,
            Err(err) => return VisitResult::Error(RuntimeError::StdError { module: "import", error: err })
        };

        if let Err(err) = runner::parse(file, path, ns.clone()).await {
            return VisitResult::Error(RuntimeError::Error(err));
        }

        /*
            Should be checking if the import exists before parseing but calling lock before a await block gets a error of 
            future cannot be sent between threads safely within `impl Future<Output = [async output]>`, the trait `Send` is not implemented for `std::sync::MutexGuard<'_, interpreter::Context>`
            required for the cast to the object type `dyn Future<Output = VisitResult<DataType>> + Send

            so await stuff needs to be done before hand
        */

        let mut lock = match ctx.lock() {
            Ok(value) => value,
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".to_string() 
            }) 
        };

        match lock.add_import(abs_str) {
            Ok(imported) => {
                if !imported {
                    return VisitResult::Error(RuntimeError::InternalError { reason: "Already imported module" })
                }
            }
            Err(err) => return VisitResult::Error(RuntimeError::Error(err))
        };

        if let Err(err) = lock.insert(namespace, DataType::NAMESPACE(ns), false) {
            return VisitResult::Error(RuntimeError::Error(err));
        }
        
        VisitResult::Ok(DataType::NULL)
    }
    async fn visit_namespace(&self, ctx: Ctx, _start: Position, _end: Position, identifer: String, scope: Box<Node>) -> VisitResult<DataType> {

        let namespace_ctx = Arc::new(
            Mutex::new(
                Context::new(Some(ctx.clone()),identifer.clone(),PathBuf::default())
            )
        );

        match self.visit(*scope, namespace_ctx.clone()).await {
            VisitResult::Error(err) => return VisitResult::Error(err),
            _ => {}
        };

        let mut lock = match ctx.lock() {
            Ok(value) => value,
            Err(_) => return VisitResult::Error(RuntimeError::ReferenceError { 
                reason: "Failed to access context".to_string() 
            }) 
        };

        match lock.insert(identifer, DataType::NAMESPACE(namespace_ctx.clone()), true) {
            Ok(value) => VisitResult::Ok(value),
            Err(err) => VisitResult::Error(RuntimeError::Error(err))
        }
    }
}