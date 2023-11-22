#include <fcgiapp.h>
#include <stdlib.h>

void main(void) {

  FCGX_Init();
  
  FCGX_Request req;
  FCGX_InitRequest(&req, 0, 0);

  while (FCGX_Accept_r(&req) >= 0) {
    FCGX_FPrintF(req.out, "Content-Type: text/html\n\n");
    FCGX_FPrintF(req.out, "Hello World!");
    FCGX_Finish_r(&req);
  }
}
