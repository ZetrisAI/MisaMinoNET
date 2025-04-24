﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace MisaMinoNET {
    static class AbortCoordinator {
        private static readonly object locker = new object();
        private static readonly HashSet<ManualResetEvent> waits = new HashSet<ManualResetEvent>();

        public static Action CreateWaiter() {
            var wait = new ManualResetEvent(false);
            lock (locker)
                waits.Add(wait);

            return () => {
                try {
                    while (true) {
                        if (wait.WaitOne(200))
                            return;

                        if (!Interface.Running) {
                            // We're in the fucking stupid ass undebuggable deadlock
                            lock (locker)
                                waits.Remove(wait);
                            return;
                        }
                    }
                } finally {
                    wait.Dispose();
                }
            };
        }

        public static void WakeWaiters() {
            ManualResetEvent[] toSignal;
            lock (locker) {
                toSignal = waits.ToArray();
                waits.Clear();
            }

            foreach (var ev in toSignal)
                ev.Set();
        }
    }
}
