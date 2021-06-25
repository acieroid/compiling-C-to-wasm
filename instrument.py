from __future__ import print_function
import sys
import copy

sys.path.extend(['.', '..'])

from pycparser import parse_file, c_generator, c_parser, c_ast, plyparser

class AssignmentVisitor(c_ast.NodeVisitor):
    to_add = set()
    def add(self, var, coord):
        self.to_add.add(c_ast.FuncCall(c_ast.ID('printf'), c_ast.ExprList([c_ast.Constant(type='string', value='"\\nORBS:%d\\n"'), c_ast.ID(var)]), plyparser.Coord(file='', line=coord.line)))
    def visit_Assignment(self, node):
        self.add(node.lvalue.name, node.coord)
    def visit_UnaryOp(self, node):
        if node.op == 'p++' or node.op == 'p--':
            self.add(node.expr.name, node.coord)

class Generator(c_generator.CGenerator):
    def __init__(self, to_add):
        self.to_add = to_add
        self.added = False
    def visit(self, node):
        s = ''
        if not self.added and node != None and node.coord != None and self.to_add != node and self.to_add.coord.line <= node.coord.line:
            self.added = True
            if isinstance(node, c_ast.For):
                # We add the instrumentation to the first line of its body
                if isinstance(node.stmt, c_ast.Compound):
                    node.stmt.block_items.insert(0, self.to_add)
                else:
                    node.stmt = c_ast.Compound([self.to_add, node.stmt])
                return super(Generator, self).visit(node)
            else:
                return '%s;\n%s%s' % (super(Generator, self).visit(node), self._make_indent(), self.visit(self.to_add))
        else:
            return super(Generator, self).visit(node)

def translate_to_c(ast, to_add):
    copy_ast = copy.deepcopy(ast)
    generator = Generator(to_add)
    print(generator.visit(copy_ast))

if __name__ == "__main__":
    if len(sys.argv) > 1:
        ast = parse_file(sys.argv[1], use_cpp=True)
        v = AssignmentVisitor()
        v.visit(ast)
        for to_add in v.to_add:
            translate_to_c(ast, to_add)
    else:
        print("Please provide a filename as argument")
