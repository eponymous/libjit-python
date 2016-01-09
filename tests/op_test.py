import unittest
import jit

class jit_binop(jit.Function):
    def __init__(self, context, op, arg_type):
        super(jit_binop, self).__init__(context)
        self.op = op
        self.arg_type = arg_type
        self.create()

    def create_signature(self):
        return jit.type.create_signature(jit.abi.cdecl, self.arg_type, [self.arg_type, self.arg_type])

    def build(self):
        left = self.get_param(0)
        right = self.get_param(1)
        tmp = getattr(self, self.op)(left, right)
        self.insn_return(tmp)

class jit_uniop(jit.Function):
    def __init__(self, context, op, arg_type):
        super(jit_uniop, self).__init__(context)
        self.op = op
        self.arg_type = arg_type
        self.create()

    def create_signature(self):
        return jit.type.create_signature(jit.abi.cdecl, self.arg_type, [self.arg_type])

    def build(self):
        val = self.get_param(0)
        tmp = getattr(self, self.op)(val)
        self.insn_return(tmp)

class JitShortTest(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7
        self.right = 2

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left + self.right)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left - self.right)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left * self.right)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left / self.right)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left % self.right)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.short)
        self.failUnlessEqual(f(self.left), -self.left)

    def test_insn_and(self):
        f = jit_binop(self.context, 'insn_and', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left & self.right)

    def test_insn_or(self):
        f = jit_binop(self.context, 'insn_or', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left | self.right)

    def test_insn_xor(self):
        f = jit_binop(self.context, 'insn_xor', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left ^ self.right)

    def test_insn_not(self):
        f = jit_uniop(self.context, 'insn_not', jit.type.short)
        self.failUnlessEqual(f(self.left),  ~self.left)

    def test_insn_shl(self):
        f = jit_binop(self.context, 'insn_shl', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left << self.right)

    def test_insn_shr(self):
        f = jit_binop(self.context, 'insn_shr', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left >> self.right)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left == self.right)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left != self.right)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left < self.right)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left <= self.right)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left > self.right)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left >= self.right)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), self.left ** self.right)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), min(self.left, self.right))

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.short)
        self.failUnlessEqual(f(self.left, self.right), max(self.left, self.right))


class JitIntTest(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7
        self.right = 2

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left + self.right)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left - self.right)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left * self.right)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left / self.right)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left % self.right)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.int)
        self.failUnlessEqual(f(self.left), -self.left)

    def test_insn_and(self):
        f = jit_binop(self.context, 'insn_and', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left & self.right)

    def test_insn_or(self):
        f = jit_binop(self.context, 'insn_or', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left | self.right)

    def test_insn_xor(self):
        f = jit_binop(self.context, 'insn_xor', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left ^ self.right)

    def test_insn_not(self):
        f = jit_uniop(self.context, 'insn_not', jit.type.int)
        self.failUnlessEqual(f(self.left),  ~self.left)

    def test_insn_shl(self):
        f = jit_binop(self.context, 'insn_shl', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left << self.right)

    def test_insn_shr(self):
        f = jit_binop(self.context, 'insn_shr', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left >> self.right)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left == self.right)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left != self.right)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left < self.right)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left <= self.right)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left > self.right)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left >= self.right)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), self.left ** self.right)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), min(self.left, self.right))

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.int)
        self.failUnlessEqual(f(self.left, self.right), max(self.left, self.right))


class JitLongTest(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7
        self.right = 2

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left + self.right)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left - self.right)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left * self.right)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left / self.right)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left % self.right)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.long)
        self.failUnlessEqual(f(self.left), -self.left)

    def test_insn_and(self):
        f = jit_binop(self.context, 'insn_and', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left & self.right)

    def test_insn_or(self):
        f = jit_binop(self.context, 'insn_or', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left | self.right)

    def test_insn_xor(self):
        f = jit_binop(self.context, 'insn_xor', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left ^ self.right)

    def test_insn_not(self):
        f = jit_uniop(self.context, 'insn_not', jit.type.long)
        self.failUnlessEqual(f(self.left),  ~self.left)

    def test_insn_shl(self):
        f = jit_binop(self.context, 'insn_shl', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left << self.right)

    def test_insn_shr(self):
        f = jit_binop(self.context, 'insn_shr', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left >> self.right)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left == self.right)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left != self.right)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left < self.right)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left <= self.right)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left > self.right)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left >= self.right)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), self.left ** self.right)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), min(self.left, self.right))

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.long)
        self.failUnlessEqual(f(self.left, self.right), max(self.left, self.right))


class JitFloat32Test(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7.7
        self.right = 2.2
        self.places = 5

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left + self.right, self.places)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left - self.right, self.places)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left * self.right, self.places)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left / self.right, self.places)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left % self.right, self.places)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left), -self.left, self.places)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left == self.right, self.places)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left != self.right, self.places)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left < self.right, self.places)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left <= self.right, self.places)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left > self.right, self.places)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left >= self.right, self.places)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left ** self.right, self.places)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), min(self.left, self.right), self.places)

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.float32)
        self.failUnlessAlmostEqual(f(self.left, self.right), max(self.left, self.right), self.places)


class JitFloat64Test(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7.7
        self.right = 2.2
        self.places = 5

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left + self.right, self.places)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left - self.right, self.places)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left * self.right, self.places)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left / self.right, self.places)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left % self.right, self.places)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left), -self.left, self.places)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left == self.right, self.places)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left != self.right, self.places)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left < self.right, self.places)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left <= self.right, self.places)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left > self.right, self.places)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left >= self.right, self.places)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left ** self.right, self.places)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), min(self.left, self.right), self.places)

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.float64)
        self.failUnlessAlmostEqual(f(self.left, self.right), max(self.left, self.right), self.places)


class JitSysFloatTest(unittest.TestCase):
    def setUp(self):
        self.context = jit.Context()
        self.left = 7.7
        self.right = 2.2
        self.places = 5

    def test_insn_add(self):
        f = jit_binop(self.context, 'insn_add', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left + self.right, self.places)

    def test_insn_sub(self):
        f = jit_binop(self.context, 'insn_sub', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left - self.right, self.places)

    def test_insn_mul(self):
        f = jit_binop(self.context, 'insn_mul', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left * self.right, self.places)

    def test_insn_div(self):
        f = jit_binop(self.context, 'insn_div', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left / self.right, self.places)

    def test_insn_rem(self):
        f = jit_binop(self.context, 'insn_rem', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left % self.right, self.places)

    def test_insn_neg(self):
        f = jit_uniop(self.context, 'insn_neg', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left), -self.left, self.places)

    def test_insn_eq(self):
        f = jit_binop(self.context, 'insn_eq', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left == self.right, self.places)

    def test_insn_ne(self):
        f = jit_binop(self.context, 'insn_ne', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left != self.right, self.places)

    def test_insn_lt(self):
        f = jit_binop(self.context, 'insn_lt', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left < self.right, self.places)

    def test_insn_le(self):
        f = jit_binop(self.context, 'insn_le', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left <= self.right, self.places)

    def test_insn_gt(self):
        f = jit_binop(self.context, 'insn_gt', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left > self.right, self.places)

    def test_insn_ge(self):
        f = jit_binop(self.context, 'insn_ge', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left >= self.right, self.places)

    def test_insn_pow(self):
        f = jit_binop(self.context, 'insn_pow', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), self.left ** self.right, self.places)

    def test_insn_min(self):
        f = jit_binop(self.context, 'insn_min', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), min(self.left, self.right), self.places)

    def test_insn_max(self):
        f = jit_binop(self.context, 'insn_max', jit.type.sys.float)
        self.failUnlessAlmostEqual(f(self.left, self.right), max(self.left, self.right), self.places)


if __name__ == '__main__':
    unittest.main()
