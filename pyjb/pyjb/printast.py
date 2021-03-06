from pprint import pformat
import ast


def ast2tree(node, include_attrs=True):
    def _transform(node):
        if isinstance(node, ast.AST):
            fields = ((a, _transform(b)) for a, b in ast.iter_fields(node))
            if include_attrs:
                attrs = (
                    (a, _transform(getattr(node, a)))
                    for a in node._attributes
                    if hasattr(node, a)
                )
                return node.__class__.__name__, dict(fields), dict(attrs)
            return node.__class__.__name__, dict(fields)
        elif isinstance(node, list):
            return [_transform(x) for x in node]
        elif isinstance(node, str):
            return repr(node)
        return node

    if not isinstance(node, ast.AST):
        raise TypeError("expected AST, got %r" % node.__class__.__name__)
    return _transform(node)


def print_formatted_ast(node, include_attrs=False, **kwargs):
    print(pformat(ast2tree(node, include_attrs), **kwargs))
