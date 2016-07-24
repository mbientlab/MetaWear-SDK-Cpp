namespace MbientLab.MetaWear.Processor {
    public class Comparator {
        public enum Operation {
            EQ = 0,
            NEQ,
            LT,
            LTE,
            GT,
            GTE
        }

        public enum Mode {
            ABSOLUTE = 0,
            REFERENCE,
            ZONE,
            BINARY
        }
    }

    public class Delta {
        public enum Mode {
            ABSOLUTE = 0,
            DIFFERENTIAL,
            BINARY
        }
    }

    public class Maths {
        public enum Operation {
            ADD = 1,
            MULTIPLY,
            DIVIDE,
            MODULUS,
            EXPONENT,
            SQRT,
            LSHIFT,
            RSHIFT,
            SUBTRACT,
            ABS_VALUE,
            CONSTANT
        }
    }

    public class Passthrough {
        public enum Mode {
            ALL = 0,
            CONDITIONAL,
            COUNT
        }
    }

    public class Pulse {
        public enum Output {
            WIDTH = 0,
            AREA,
            PEAK,
            ON_DETECTION
        }
    }

    public class Threshold {
        public enum Mode {
            ABSOLUTE = 0,
            BINARY
        }
    }

    public class Time {
        public enum Mode {
            ABSOLUTE = 0,
            DIFFERENTIAL
        }
    }
}
