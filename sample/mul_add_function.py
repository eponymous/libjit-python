import jit

class mul_add_function(jit.Function):
    def __init__(self, context):
        super(mul_add_function, self).__init__(context)
        self.create()

    def create_signature(self):
        return jit.type.create_signature(jit.abi.cdecl, jit.type.int, [jit.type.int, jit.type.int, jit.type.int])

    def build(self):
        x, y, z = self.params

        self.insn_return(x * y + z)


if __name__ == '__main__':
    c = jit.Context()
    f = mul_add_function(c)
    print f(3, 5, 2)
    print f.dump()

