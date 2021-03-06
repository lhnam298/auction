#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "Manage.h"

Program *program;
int count_id;

void updateID(void) {
  FILE *f;
  f = fopen("ID.txt", "w");
  fprintf(f, "%d", count_id);
  fclose(f);
}

int checkChangePass(Account person, char *s) {
  AccountList *acclist;
  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp((acclist->account).user, person.user) == 0) {
      strcpy((acclist->account).pass, s);
      break;
    }
    acclist = acclist->next;
  }
  updateAccount();
  return 1;
}

void updateProduct(void) {
  FILE *f;
  Product product;
  ProductList *prolist;
  f = fopen("Product.txt", "w");
  prolist = program->proList;
  while (prolist != NULL) {
    product = prolist->product;
    fwrite(&product, sizeof(Product), 1, f);
    prolist = prolist->next;
  }
  fclose(f);
}

void addProduct(Product _product) {
  ProductList *prolist;

  prolist = (ProductList*)malloc(sizeof(ProductList));
  prolist->product = _product;
  prolist->next = NULL;
  if (program->proList == NULL)
    program->proList = prolist;
  else {
    prolist->next = program->proList;
    program->proList = prolist;
  }
  updateProduct();
}

int Detail(Product *_product, int id) {
  ProductList *prolist;
  prolist = program->proList;
  while (prolist != NULL) {
    if ((prolist->product).id == id) {
      *_product = prolist->product;
      return 1;
    }
    prolist = prolist->next;
  }
  return 0;
}

int count(int key) {
  AccountList *acclist;
  ProductList *prolist;
  int k = 0;
  if (key == 1) {
    acclist = program->accList;
    while (acclist != NULL) {
      k++;
      acclist = acclist->next;
    }
    return k-1;
  }
  else {
    prolist = program->proList;
    while (prolist != NULL) {
      k++;
      prolist = prolist->next;
    }
    return k;
  }
}

int deleteAccount(char *s) {
  AccountList *acclist, *cur;
  if (strcmp(s, "admin") == 0)
    return 0;
  acclist = program->accList;
  if (strcmp((acclist->account).user, s) == 0) {
    program->accList = acclist->next;
    free(acclist);
    updateAccount();
    return 1;
  }
  else {
    while (acclist->next != NULL) {
      if (strcmp(((acclist->next)->account).user, s) == 0) {
        cur = acclist->next;
        acclist->next = cur->next;
        free(cur);
        updateAccount();
        return 1;
      }
      acclist = acclist->next;
    }
  }
  return 0;
}

void Logout(Account person) {
  AccountList *acclist;
  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp((acclist->account).user, person.user) == 0) {
      (acclist->account).onl = 0;
      break;
    }
    acclist = acclist->next;
  }
  return;
}

int setCoin(char *s, int money) {
  AccountList *acclist;
  int k;
  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp((acclist->account).user, s) == 0) {
      (acclist->account).coin += money;
      updateAccount();
      return 1;
    }
    acclist = acclist->next;
  }
  return 0;
}

void updateAccount(void) {
  FILE *f;
  Account person;
  AccountList *acclist;

  f = fopen("Account.txt", "w");
  acclist = program->accList;
  while (acclist != NULL) {
    person = acclist->account;
    if (person.onl == 1)
      person.onl = 0;
    fwrite(&person, sizeof(Account), 1, f);
    acclist = acclist->next;
  }
  fclose(f);
}

int checkRegister(Account person) {
  AccountList *acclist;
  FILE *f;

  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp(person.user, (acclist->account).user) == 0)
      return -1;
    acclist = acclist->next;
  }

  acclist = (AccountList*)malloc(sizeof(AccountList));
  acclist->account = person;
  acclist->next = program->accList;
  program->accList = acclist;
  f = fopen("Account.txt","a");
  if (f == NULL) return 0;
  fwrite(&person, sizeof(Account), 1, f);
  fclose(f);
  return 1;
}

Account checkLogin(Account person, int *flag) {
  AccountList *acclist;
  *flag = 0;
  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp(person.user, (acclist->account).user) == 0
        && strcmp(person.pass, (acclist->account).pass) == 0)
      if ((acclist->account).onl == 0) {
        (acclist->account).onl = 1;
        *flag = 1;
        return acclist->account;
      }
      else {
        *flag = -1;
        return;
      }
    acclist = acclist->next;
  }
  return;
}

void init(int *count_id) {
  FILE *f;
  Account account;
  Product product;
  AccountList *acclist;
  ProductList *prolist;
  program = (Program*)malloc(sizeof(Program));
  program->accList = NULL;
  program->proList = NULL;

  f = fopen("Account.txt", "r");
  while (fread(&account, sizeof(Account), 1, f) > 0) {
    acclist = (AccountList*)malloc(sizeof(AccountList));
    acclist->account = account;
    acclist->next = NULL;

    if (program->accList == NULL)
      program->accList = acclist;
    else {
      acclist->next = program->accList;
      program->accList = acclist;
    }
  }
  fclose(f);

  /*************************************************/

  f = fopen("Product.txt", "r");
  if (f != NULL) {
    while (fread(&product, sizeof(Product), 1, f) > 0) {
      prolist = (ProductList*)malloc(sizeof(ProductList));
      prolist->product = product;
      prolist->next = NULL;

      if (program->proList == NULL)
        program->proList = prolist;
      else {
        prolist->next = program->proList;
        program->proList = prolist;
      }
    }
    fclose(f);
  }

  /*************************************************/

  f = fopen("ID.txt", "r");
  fscanf(f, "%d", &(*count_id));
  fclose(f);
}

int checkTime(void) {
  Product _product;
  time_t current_time;
  if (program->proList == NULL) return 0;
  _product = (program->proList)->product;
  if (_product.status == 0)
    return 0;
  time(&current_time);
  if (current_time > _product.finish)
    return 1;
  return 0;
}

void finishAuction(void) {
  Product _product;
  AccountList *acclist;
  boughtList *new;
  _product = (program->proList)->product;
  _product.status = 0;
  if (strcmp(_product.master, "") == 0) {
    (program->proList)->product = _product;
    updateProduct();
    return;
  }
  acclist = program->accList;
  while (acclist != NULL) {
    if (strcmp((acclist->account).user, _product.master) == 0) {
      (acclist->account).coin = (acclist->account).coin - _product.max_price;
      new = (boughtList*)malloc(sizeof(boughtList));
      new->id = _product.id;
      new->next = NULL;
      if ((acclist->account).listid == NULL)
        (acclist->account).listid = new;
      else {
        new->next = (acclist->account).listid;
        (acclist->account).listid = new;
      }
      break;
    }
    acclist = acclist->next;
  }
  (program->proList)->product = _product;
  updateProduct();
  updateAccount();
}

void process(int sockfd)
{
  int n, k, flag, length, id;
  char sendbuff[MAXLINE], recvbuff[MAXLINE];
  Account person, _person;
  Product _product;
  AccountList *acclist;
  ProductList *prolist;

  if (checkTime() == 1)
    finishAuction();

  read(sockfd, &n, sizeof(int));

  switch (n) {

  case REGISTER:
    read(sockfd, &person, sizeof(Account));
    k = checkRegister(person);
    switch (k) {
    case -1:
      strcpy(sendbuff, "Ten dang nhap da duoc su dung\n");
      break;
    case 0:
      strcpy(sendbuff, "Tam thoi khong luu tru duoc du lieu, hay thu lai sau\n");
      break;
    case 1:
      strcpy(sendbuff, "Dang ky thanh cong\n");
      break;
    }
    write(sockfd, sendbuff, strlen(sendbuff));
    break;

  case LOGIN:
    read(sockfd, &person, sizeof(Account));
    _person = checkLogin(person, &flag);
    switch (flag) {
    case -1:
      strcpy(sendbuff, "Tai khoan dang dang nhap tai mot may khac, hay thu lai sau\n");
      break;
    case 0:
      strcpy(sendbuff, "Sai ten dang nhap hoac mat khau\n");
      break;
    case 1:
      strcpy(sendbuff, "Dang nhap thanh cong\n");

      break;
    }
    write(sockfd, &flag, sizeof(int));
    write(sockfd, &_person, sizeof(Account));
    write(sockfd, sendbuff, strlen(sendbuff));
    break;

  case LIST_ACCOUNT:
    k = count(1);
    write(sockfd, &k, sizeof(int));
    acclist = program->accList;
    while (acclist != NULL) {
      if (strcmp((acclist->account).user, "admin") != 0) {
        person = acclist->account;
        write(sockfd, &person, sizeof(Account));
      }
      acclist = acclist->next;
    }
    break;

  case DELETE_ACCOUNT:
    length = read(sockfd, recvbuff, MAXLINE);
    read(sockfd, &k, sizeof(int));
    if (k == 0) return;
    recvbuff[length] = '\0';
    k = deleteAccount(recvbuff);
    if (k == 1)
      strcpy(sendbuff, "Da xoa\n");
    else
      strcpy(sendbuff, "Khong tim thay tai khoan trong danh sach\n");
    write(sockfd, sendbuff, strlen(sendbuff));
    break;

  case SETCOIN:
    length = read(sockfd, recvbuff, MAXLINE);
    read(sockfd, &k, sizeof(int));
    recvbuff[length] = '\0';
    if (setCoin(recvbuff, k) == 1)
      strcpy(sendbuff, "Cap nhat so du tai khoan thanh cong\n");
    else
      strcpy(sendbuff, "Khong tim thay tai khoan trong danh sach\n");
    write(sockfd, sendbuff, strlen(sendbuff));
    break;

  case CHANGE_PASSWORD:
    read(sockfd, &person, sizeof(Account));
    length = read(sockfd, recvbuff, MAXLINE);
    recvbuff[length] = '\0';
    k = checkChangePass(person, recvbuff);
    write(sockfd, &k, sizeof(int));
    break;

  case LOGOUT:
    read(sockfd, &person, sizeof(Account));
    Logout(person);
    break;

  case LIST_PRODUCT:
    k = count(2);
    write(sockfd, &k, sizeof(int));
    prolist = program->proList;
    while (prolist != NULL) {
      _product = prolist->product;
      write(sockfd, &_product, sizeof(Product));
      prolist = prolist->next;
    }
    break;

  case ADD_PRODUCT:
    prolist = program->proList;
    k = 0;
    if (prolist != NULL)
      k = (prolist->product).status;
    write(sockfd, &k, sizeof(int));
    if (k == 1) return;
    read(sockfd, &_product, sizeof(Product));
    _product.id = ++count_id;
    _product.status = 1;
    addProduct(_product);
    updateID();
    break;

  case DETAIL_PRODUCT:
    read(sockfd, &id, sizeof(int));
    k = Detail(&_product, id);
    write(sockfd, &k, sizeof(int));
    if (k == 0) {
      strcpy(sendbuff, "Khong tim thay san pham co id tren\n");
      write(sockfd, sendbuff, strlen(sendbuff));
      return;
    }
    write(sockfd, &_product, sizeof(Product));
    break;

  case UPDATE:
    if (program->proList == NULL) k = -1;
    else {
      _product = (program->proList)->product;
      k = _product.status;
    }
    write(sockfd, &k, sizeof(int));
    if (k == 1)
      write(sockfd, &_product, sizeof(Product));
    else
      if (k == -1) {
        strcpy(sendbuff, "Chua co san pham nao duoc dau gia\n");
        write(sockfd, sendbuff, strlen(sendbuff));
      }
      else {
        write(sockfd, &_product, sizeof(Product));
        strcpy(sendbuff, "Phien dau gia da ket thuc\n");
        write(sockfd, sendbuff, strlen(sendbuff));
      }
    break;

  case AUCTIONS:
    if (program->proList == NULL) k = 0;
    else {
      _product = (program->proList)->product;
      k = _product.status;
    }
    write(sockfd, &k, sizeof(int));
    if (k == 0) return;
    read(sockfd, &flag, sizeof(int));
    if (flag == 0) return;
    read(sockfd, &k, sizeof(int));
    length = read(sockfd, recvbuff, MAXLINE);
    recvbuff[length] = '\0';
    if (k < _product.max_price ||
        (k == _product.max_price && strcmp(_product.master, "") !=0 )) {
      strcpy(sendbuff, "Ban phai dat gia lon hon gia cao nhat hien tai\n");
      write(sockfd, sendbuff, strlen(sendbuff));
    }
    else {
      _product.max_price = k;
      strcpy(_product.master, recvbuff);
      _product.finish += 10;
      (program->proList)->product = _product;
      strcpy(sendbuff, "Dat gia thanh cong\n");
      write(sockfd, sendbuff, strlen(sendbuff));
      updateProduct();
    }
    break;

  case UPDATE_ACCOUNT:
    read(sockfd, &person, sizeof(Account));
    acclist = program->accList;
    while (acclist != NULL) {
      if (strcmp((acclist->account).user, person.user) == 0) {
        person = acclist->account;
        write(sockfd, &person, sizeof(Account));
        return;
      }
      acclist = acclist->next;
    }
    break;

  case BOUGHT_PRODUCTS:
    for (;;) {
      read(sockfd, &k, sizeof(int));
      if (k < 0) break;
      prolist = program->proList;
      while (prolist != NULL) {
        if ((prolist->product).id == k) {
          _product = prolist->product;
          write(sockfd, &_product, sizeof(Product));
          break;
        }
        prolist = prolist->next;
      }
    }
    break;

  default:
    return;
  }
}

int
main() {
  int client[FD_SETSIZE];
  int recv, fd, k=0;   //bien k danh dau dem lan 1,lan2...
  int nread, i, t=-120,result;
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  fd_set readfds, testfds;
  struct timeval timeout;
  init(&count_id);
  timeout.tv_sec = 1;
  timeout.tv_usec = 1000;

  //tao va dinh cong cho socket cua trinh chu lang nghe ket noi

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_sockfd == 0){
    perror("\t\t | socket() error!\n");
    return 0;
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(5000);
  if(bind(server_sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
    perror("\t\t | bind() error\n");
    close(server_sockfd);
    return 0;
  }

  if(listen(server_sockfd,10) != 0){
    printf("\t\t | listen() error\n");
    close(server_sockfd);
    return 0;
  }
  FD_ZERO(&readfds);
  FD_ZERO(&testfds);
  // Dat socket cua trinh chu vao danh sach va buoc vao vong lap cho ket noi

  FD_SET(server_sockfd, &readfds);

  for (;;) {
    testfds = readfds;
    //Ham select cho tin hieu xuat nhap gui den 1 trong cac socket trong danh sach testfds

    result = select(FD_SETSIZE, &testfds, NULL, NULL, &timeout);
    if(result == -1){
      printf("\t\t | select() error\n");
      close(server_sockfd);
      return 0;
    }
    if(result == 0) {
      if(t != -1) {
        t++;
        printf("\n%d",t);
      }
      timeout.tv_sec = timeout.tv_sec+1;
    }

    for (fd = 0; fd < FD_SETSIZE; fd++) {
      if(FD_ISSET(fd, &testfds)) {
        //Neu la socket cua trinh chu
        //client moi tham gia ket noi,chua dang nhap
        if(fd == server_sockfd) {

          client_sockfd = accept(server_sockfd, NULL, NULL);

          FD_SET(client_sockfd, &readfds);
          printf("\t\t | PID = %d  da ket noi thanh cong\n", client_sockfd-3);
          printf("\t\t | Cho Client dang nhap\n");

        }
        else {
          ioctl(fd, FIONREAD, &nread);
          //Dong ket noi neu khong con du lieu de doc
          if(nread == 0){
            close(fd);
            //Loai bo socket trinh khach ra khoi danh sach cho
            FD_CLR(fd, &readfds);
            //xoa client ra khoi hang doi
            for (i = 0; i < FD_SETSIZE; i++)
              if (client[i] == fd) {
                client[i] = -1;
                break;
              }
            printf("\t\t | %d Ngat ket noi voi PID = %d\n",t,fd-3);

          }
          else
            process(fd);
        }
      }
    }
  }
}
