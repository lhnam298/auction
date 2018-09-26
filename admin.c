#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Manage.h"

main() {
  Account admin;
  FILE *f;
  strcpy(admin.user, "admin");
  strcpy(admin.pass, "123456");
  strcpy(admin.email, "admin_project@gmail.com");
  strcpy(admin.addr, "AS_K55");
  strcpy(admin.phone, "01669806628");
  admin.onl = 0;	
  admin.lvl = -1;
  admin.coin = 0;
  admin.listid = NULL;
  f = fopen("Account.txt","w");
  fwrite(&admin, sizeof(Account), 1, f);
  fclose(f);
}
