using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

using MisaMinoNET;

namespace MisaMinoNET {
    public class Interface {
        [DllImport("libtetris_ai.dll")]
        public static extern int test();
    }

    public class MisaMino {
        public static int test() {
            return Interface.test();
        }
    }
}
