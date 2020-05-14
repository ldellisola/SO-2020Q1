#include <stdint.h>
#include "../include/idtLoader.h"
#include "../include/defs.h"
#include "../include/interrupts.h"

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */



DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

static void setup_IDT_entry (int index, uint64_t offset);

void load_idt() {

  setup_IDT_entry (0x00, (uint64_t)&_exception0Handler);
  setup_IDT_entry (0x06, (uint64_t)&_exception6Handler);
  setup_IDT_entry (0x04, (uint64_t)&_exception4Handler);
  setup_IDT_entry (0x08, (uint64_t)&_exception8Handler);
  setup_IDT_entry (0x0C, (uint64_t)&_exception0xCHandler);
  setup_IDT_entry (0x0d, (uint64_t)&_exception0xDHandler);
  setup_IDT_entry (0x0e, (uint64_t)&_exception0xEHandler);

  setup_IDT_entry (0x20, (uint64_t)&_irq00Handler);
  setup_IDT_entry (0x21, (uint64_t)&_irq01Handler);
  setup_IDT_entry(0x80,(uint64_t)&_irq80Handler);
  setup_IDT_entry(0x81,(uint64_t)&_irq81Handler);
  setup_IDT_entry(0x82,(uint64_t)&_irq82Handler);
  setup_IDT_entry(0x86,(uint64_t)&_irq86Handler);
  setup_IDT_entry(0x87,(uint64_t)&_irq87Handler);
  setup_IDT_entry(0x88,(uint64_t)&_irq88Handler);
  setup_IDT_entry(0x89,(uint64_t)&_irq89Handler);
  setup_IDT_entry(0x90,(uint64_t)&_irq90Handler);
  setup_IDT_entry(0x91,(uint64_t)&_irq91Handler);
  setup_IDT_entry(0x92,(uint64_t)&_irq92Handler);
  setup_IDT_entry(0x93,(uint64_t)&_irq93Handler);
  setup_IDT_entry(0x94,(uint64_t)&_irq94Handler);
  setup_IDT_entry(0x95,(uint64_t)&_irq95Handler);
  setup_IDT_entry(0x96,(uint64_t)&_irq96Handler);
  setup_IDT_entry(0x97,(uint64_t)&_irq97Handler);
  setup_IDT_entry(0x98,(uint64_t)&_irq98Handler);
	
  picMasterMask(0xFC); 
	picSlaveMask(0xFF);
        
	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}
