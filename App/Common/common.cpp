#include "SBT-SDK.hpp"
#include <FreeRTOS.h>

extern "C" {
__attribute__((aligned(64))) uint8_t ucHeap[configTOTAL_HEAP_SIZE];
}

// For static constructors
extern "C" {
void _init() {}
void __libc_init_array();
}

extern "C" {
// Ranges of the .bss section
extern size_t __bss_start[];
extern size_t __bss_end[];

// Starting point of the data being values for static variables
extern size_t __data_load[];
// Location of the static variables
extern size_t __data_start[];
extern size_t __data_end[];

// Static object destructor info, left unused
extern size_t __dso_handle[];

// Substitute common functions because linker links with --nostartfiles
void* memset(void* dest, int c, size_t n) {
  unsigned char* s = (unsigned char*)dest;
  size_t k;

  /* Fill head and tail with minimal branching. Each
   * conditional ensures that all the subsequently used
   * offsets are well-defined and in the dest region. */

  if (!n)
    return dest;
  s[0] = s[n - 1] = c;
  if (n <= 2)
    return dest;
  s[1] = s[n - 2] = c;
  s[2] = s[n - 3] = c;
  if (n <= 6)
    return dest;
  s[3] = s[n - 4] = c;
  if (n <= 8)
    return dest;

  /* Advance pointer to align it at a 4-byte boundary,
   * and truncate n to a multiple of 4. The previous code
   * already took care of any head/tail that get cut off
   * by the alignment. */

  k = -(uintptr_t)s & 3;
  s += k;
  n -= k;
  n &= -4;
  n /= 4;

  uint32_t* ws = (uint32_t*)s;
  uint32_t wc = c & 0xFF;
  wc |= ((wc << 8) | (wc << 16) | (wc << 24));

  /* Pure C fallback with no aliasing violations. */
  for (; n; n--, ws++)
    *ws = wc;

  return dest;
}

void* memcpy(void* destination, const void* source, size_t num) {
  for (size_t idx = 0; idx < num; ++idx) {
    *(static_cast<char*>(destination) + idx) =
        *(static_cast<const char*>(source) + idx);
  }
  return destination;
}
}

// FreeRTOS callbacks that will be used below
extern "C" {
void* pvPortMalloc(size_t);
void vPortFree(void*);

void vPortSVCHandler();
void xPortPendSVHandler();
}

// Semihosting stuff
extern "C" {
extern void initialise_monitor_handles(void);
}

// Substitute calls for dynamic memory with FreeRTOS syscalls
void* operator new(size_t size) { return pvPortMalloc(size); }

void operator delete(void* ptr) { vPortFree(ptr); }

void operator delete(void* ptr, [[maybe_unused]] size_t) { vPortFree(ptr); }

extern void entryPoint();

// First function that is called after boot-up
void resetHandler() {
  // Zero the default-initialized data
  memset(__bss_start, 0,
         reinterpret_cast<unsigned>(__bss_end) -
             reinterpret_cast<unsigned>(__bss_start));

  // Initialize static data
  memcpy(__data_start, __data_load,
         reinterpret_cast<unsigned>(__data_end) -
             reinterpret_cast<unsigned>(__data_start));

  // Enable semihosting
  //    initialise_monitor_handles();

  /* Call static constructors */
  __libc_init_array();

  entryPoint();
}

// Handler for 1ms interrupt
void systick() { SBT::System::SystickHandler(); }

// When something goes wrong
void hardfault() {
  while (true)
    ; // OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo!
}

// Callbacks for FreeRTOS supervisor calls
[[gnu::naked]] void svcHandler() { asm volatile("b vPortSVCHandler"); }
[[gnu::naked]] void pendSvcHandler() { asm volatile("b xPortPendSVHandler"); }
