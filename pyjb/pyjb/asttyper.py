from textwrap import dedent
from pyjb.typedast import *

import ast
import inspect
import types


prim_ops = {ast.Add: "add", ast.Sub: "sub", ast.Div: "div", ast.Mult: "mult"}


class AstTyper(ast.NodeVisitor):
    def __init__(self):
        self.types = {}

    def __call__(self, source):
        if isinstance(source, types.ModuleType):
            source = dedent(inspect.getsource(source))
        if isinstance(source, types.FunctionType):
            source = dedent(inspect.getsource(source))
        if isinstance(source, types.LambdaType):
            source = dedent(inspect.getsource(source))
        elif isinstance(source, (str, types)):
            source = dedent(source)
        else:
            raise NotImplementedError

        self._source = source
        self._ast = ast.parse(source)
        return self.visit(self._ast)

    def visit_Module(self, node):
        body = list(map(self.visit, node.body))
        return body[0]

    def visit_Name(self, node):
        return Var(node.id, self.types.get(node.id, None))

    def visit_If(self, node):
        body = list(map(self.visit, node.body))
        orelse = list(map(self.visit, node.orelse))
        test = self.visit(node.test)

        return If(body, orelse, test)

    def visit_For(self, node):
        target = self.visit(node.target)

        # hack, but not really
        target.type = "int64"

        body = list(map(self.visit, node.body))
        if node.iter.func.id in ("xrange", "range"):
            args = list(map(self.visit, node.iter.args))
        else:
            raise Exception("Loop must be over range")

        if len(args) == 1:  # range(n)
            return For(target, Int(0, "int64"), args[0], Int(1, "int64"), body)
        elif len(args) == 2:  # range(n,m)
            return For(target, args[0], args[1], Int(1, "int64"), body)
        elif len(args) == 3:
            return For(target, args[0], args[1], args[2], body)

    def visit_While(self, node):
        body = [self.visit(statement) for statement in node.body]
        test = self.visit(node.test)

        return While(body, test)

    def visit_FunctionDef(self, node):
        def helper(arg, type_):
            type_ = (
                type_.id
                if not isinstance(type_, ast.Subscript)
                else f"array({type_.slice.value.id})"
            )
            self.types[arg] = type_
            return Var(arg, type_)

        # self.types.update({a.arg: a.annotation.id for a in node.args.args})

        args = [helper(a.arg, a.annotation) for a in node.args.args]

        body = list(node.body)
        body = list(map(self.visit, body))

        return FunctionDef(
            node.name,
            args,
            body,
            node.returns.id if hasattr(node.returns, "id") else None,
        )

    def visit_Pass(self, node):
        return NoOp()

    def visit_Return(self, node):
        value = self.visit(node.value)
        return Return(value)

    def visit_Expr(self, node):
        print("PASSED EXPR")
        pass

    def visit_BinOp(self, node):
        op_name = prim_ops[node.op.__class__]
        left = self.visit(node.left)
        right = self.visit(node.right)
        return Prim(op_name, [left, right])

    def visit_Constant(self, node):
        if isinstance(node.n, float):
            return Float(node.value, "float64")
        elif isinstance(node.n, int):
            return Int(node.value, "int64")

    def visit_Bool(self, node):
        return Bool(node.n)

    def visit_Call(self, node):
        name = self.visit(node.func)
        args = list(map(self.visit, node.args))
        return Call(name, args)

    def visit_Assign(self, node):
        assert len(node.targets) == 1

        target = self.visit(node.targets[0])
        value = self.visit(node.value)

        return Assign(target, value)

    # TODO: This needs fixing for array types
    def visit_AnnAssign(self, node):
        target = node.target.id
        type_ = node.annotation.id
        value = self.visit(node.value)
        self.types[target] = type_
        return Assign(target, value, type_)

    def visit_AugAssign(self, node):
        target = (
            node.target.id
            if not isinstance(node.target, ast.Subscript)
            else self.visit(node.target)
        )
        type_ = self.types.get(target, None)
        value = self.visit(node.value)
        if isinstance(node.op, ast.Add):
            return Assign(target, Prim("add", [Var(target, type_), value]), type_)
        if isinstance(node.op, ast.Mult):
            return Assign(target, Prim("mult", [Var(target, type_), value]), type_)
        else:
            raise NotImplementedError

    def visit_Subscript(self, node):
        if node.slice:
            value = self.visit(node.value)
            idx = self.visit(node.slice.value)
            return Index(
                value, idx, "load" if isinstance(node.ctx, ast.Load) else "store"
            )
        # elif isinstance(node.ctx, ast.Store):
        # raise NotImplementedError

    def visit_Attribute(self, node):
        if node.attr == "shape":
            value = self.visit(node.value)
            return Prim("shape", [value])
        else:
            raise NotImplementedError

    def visit_Compare(self, node):
        comparator = self.visit(node.comparators[0])
        left = self.visit(node.left)
        op = self.visit(node.ops[0])
        return Compare(comparator, left, op)

    def visit_BoolOp(self, node):
        return BoolOp(self.visit(node.op), [self.visit(v) for v in node.values])

    def visit_And(self, node):
        return And()

    def visit_Or(self, node):
        return Or()

    def visit_NotEq(self, node):
        return NotEq()

    def visit_Eq(self, node):
        return Eq()

    def visit_Lt(self, node):
        return Lt()

    def visit_LtE(self, node):
        return LtE()

    def visit_Gt(self, node):
        return Gt()

    def visit_GtE(self, node):
        return GtE()

    def generic_visit(self, node):
        raise NotImplementedError(type(node).__name__)

