from ctypes import c_float, c_short, Structure

class CartesianFloat(Structure):
    _fields_= [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

    # taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation
    def is_close(fst, snd):
        return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.0 )

    def __eq__(self, other):
        return CartesianFloat.is_close(self.x, other.x) and CartesianFloat.is_close(self.y, other.y) and \
                CartesianFloat.is_close(self.z, other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%.3f, %.3f, %.3f)" % (self.x, self.y, self.z)

class CartesianShort(Structure):
    _fields_= [
        ("x", c_short),
        ("y", c_short),
        ("z", c_short)
    ]

    def __eq__(self, other):
        return (self.x == other.x) and (self.y == other.y) and (self.z == other.z)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        return "(%d, %d, %d)" % (self.x, self.y, self.z)

