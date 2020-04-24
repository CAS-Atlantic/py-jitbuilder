import ast


class Var(ast.AST):
    _fields = ["id", "type"]

    def __init__(self, id, type=None):
        self.id = id
        self.type = type


class Assign(ast.AST):
    _fields = ["target", "value", "type"]

    def __init__(self, target, value, type=None):
        self.target = target
        self.value = value
        self.type = type


class Return(ast.AST):
    _fields = ["value"]

    def __init__(self, value):
        self.value = value


class For(ast.AST):
    _fields = ["target", "begin", "end", "step", "body"]

    def __init__(self, target, begin, end, step, body):
        self.target = target
        self.begin = begin
        self.end = end
        self.step = step
        self.body = body


class While(ast.AST):
    _fields = ["test", "body"]

    def __init__(self, body, test):
        self.test = test
        self.body = body


class Call(ast.AST):
    _fields = ["fn", "args"]

    def __init__(self, fn, args):
        self.fn = fn
        self.args = args


class FunctionDef(ast.AST):
    _fields = ["fname", "args", "body", "return_type"]

    def __init__(self, fname, args, body, return_type):
        self.fname = fname
        self.args = args
        self.body = body
        self.return_type = return_type


class Int(ast.AST):
    _fields = ["n", "type"]

    def __init__(self, n, type_=None):
        self.n = n
        self.type = type


class Float(ast.AST):
    _fields = ["n", "type"]

    def __init__(self, n, type_=None):
        self.n = n
        self.type = type


class Bool(ast.AST):
    _fields = ["n", "type"]

    def __init__(self, n):
        self.n = n
        self.type = type


class Prim(ast.AST):
    _fields = ["fn", "args"]

    def __init__(self, fn, args):
        self.fn = fn
        self.args = args

class BoolOp(ast.AST):
    _fields = ["op", "values"]

    def __init__(self, op, values):
        self.op = op
        self.values = values

class Compare(ast.AST):
    _fields = ["comparator", "left", "op"]

    def __init__(self, comparator, left, op):
        self.comparator = comparator
        self.left = left
        self.op = op

class And(ast.AST):
    _fields = []

class Or(ast.AST):
    _fields = []

class Gt(ast.AST):
    _fields = []

class GtE(ast.AST):
    _fields = []

class Lt(ast.AST):
    _fields = []

class LtE(ast.AST):
    _fields = []

class Eq(ast.AST):
    _fields = []

class NotEq(ast.AST):
    _fields = []

class Index(ast.AST):
    _fields = ["value", "idx", "ctx"]

    def __init__(self, value, idx, ctx):
        self.value = value
        self.idx = idx
        self.ctx = ctx

class If(ast.AST):
    _fields = ["body", "orelse", "test"]

    def __init__(self, body, orelse, test):
        self.body = body
        self.orelse = orelse
        self.test = test


class NoOp(ast.AST):
    _fields = []
