from __future__ import print_function
import sys
import copy

sys.path.extend(['.', '..'])

from pycparser import parse_file, c_generator, c_parser, c_ast, plyparser

class AssignmentVisitor(c_ast.NodeVisitor):
    to_add = set()
    types = {}
    def add(self, var, coord):
        s = ''
        if var in self.types and self.types[var] == 'string':
            s = '"\\nORBS:%s\\n"'
        else:
            s = '"\\nORBS:%x\\n"'
        self.to_add.add(c_ast.FuncCall(c_ast.ID('printf'), c_ast.ExprList([c_ast.Constant(type='string', value=s), c_ast.ID(var)]), plyparser.Coord(file='', line=coord.line)))
    def visit_Assignment(self, node):
        self.add(node.lvalue.name, node.coord)
    def visit_Decl(self, node):
        if isinstance(node.type, c_ast.PtrDecl) and isinstance(node.type.type, c_ast.TypeDecl) and isinstance(node.type.type.type, c_ast.IdentifierType) and 'char' in node.type.type.type.names:
            # char*
            self.types[node.name] = 'string'
        # Other types are not stored in self.types, as we will simply print their hex value
    def visit_UnaryOp(self, node):
        if node.op == 'p++' or node.op == 'p--':
            self.add(node.expr.name, node.coord)

class Generator(c_generator.CGenerator):
    def __init__(self, to_add):
        self.to_add = to_add
        self.added = False
        self.added_type = 'expr'
    def visit(self, node):
        s = ''
        if not self.added and node != None and node.coord != None and self.to_add != node and self.to_add.coord.line <= node.coord.line:
            self.added = True
            if isinstance(node, c_ast.For):
                # We add the instrumentation to the first line of its body
                if isinstance(node.stmt, c_ast.Compound):
                    if node.stmt.block_items == None:
                        node.stmt.block_items = [self.to_add]
                    else:
                        node.stmt.block_items.insert(0, self.to_add)
                else:
                    node.stmt = c_ast.Compound([self.to_add, node.stmt])
                self.added_type = 'for'
                return super(Generator, self).visit(node)
            else:
                return '%s;\n%s%s' % (super(Generator, self).visit(node), self._make_indent(), self.visit(self.to_add))
        else:
            return super(Generator, self).visit(node)

def translate_to_c(filename, ast, to_add):
    copy_ast = copy.deepcopy(ast)
    generator = Generator(to_add)
    transformed_source = generator.visit(copy_ast)
    instrumented_filename = '%s_%s_%d_%s.c' % (filename[:-2], to_add.args.exprs[1].name, to_add.coord.line, generator.added_type)
    print(instrumented_filename)
    f = open(instrumented_filename, 'w')
    f.write(transformed_source)
    f.close()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename = sys.argv[1]
        ast = parse_file(filename, use_cpp=True)
        v = AssignmentVisitor()
        v.visit(ast)
        for to_add in v.to_add:
            translate_to_c(filename, ast, to_add)
    else:
        print("Please provide a filename as argument")
