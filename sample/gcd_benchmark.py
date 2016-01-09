import jit
import timeit


class jit_gcd(jit.Function):
    def __init__(self, context):
        super(jit_gcd, self).__init__(context)
        self.create()

    def create_signature(self):
        return jit.type.create_signature(jit.abi.cdecl, jit.type.int, [jit.type.int, jit.type.int])

    def build(self):
        x, y = self.params

        label1 = jit.Label()
        label2 = jit.Label()

        self.insn_branch_if_not(x == y, label1)
        self.insn_return(x)

        self.insn_label(label1)
        self.insn_branch_if_not(x < y, label2)
        tmp1 = self.insn_call("gcd", self, self.signature, [x, y - x])
        self.insn_return(tmp1)

        self.insn_label(label2)
        tmp2 = self.insn_call("gcd", self, self.signature, [x - y, y])
        self.insn_return(tmp2)


def gcd(x, y):
    if x == y:
        return x
    elif x < y:
        return gcd(x, y - x)
    else:
        return gcd(x - y, y)


def gcd2(x, y):
    while x != y:
        if x < y:
            y -= x
        else:
            x -= y
    return x

c = jit.Context()

if __name__ == '__main__':
    print 'gcd:      ', gcd(3430, 5719), ' ', timeit.timeit('gcd(3430, 5719)', setup='from __main__ import gcd', number=10000)
    print 'gcd2:     ', gcd2(3430, 5719), ' ', timeit.timeit('gcd2(3430, 5719)', setup='from __main__ import gcd2', number=10000)
    print 'jit_gcd:  ', jit_gcd(c)(3430, 5719), ' ', timeit.timeit('f(3430, 5719)', setup='from __main__ import jit_gcd, c; f = jit_gcd(c)', number=10000)


