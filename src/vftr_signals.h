#ifndef VFTR_SIGNALS_H
#define VFTR_SIGNALS_H

extern int vftr_signal_number;

void vftr_define_signal_handlers ();
void vftr_sigaction (int sig);

#endif
