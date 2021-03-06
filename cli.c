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

char* _asctime(const struct tm *timeptr)
{
  static const char wday_name[][9] = {
    "Chu nhat", "Thu Hai", "Thu Ba", "Thu Tu", "Thu Nam", "Thu Sau", "Thu Bay"
  };
  static const char mon_name[][15] = {
    "Thang Mot", "Thang Hai", "Thang Ba", "Thang Tu", "Thang Nam", "Thang Sau",
    "Thang Bay", "Thang Tam", "Thang Chin", "Thang Muoi", "Thang Muoi Mot", "Thang Muoi Hai"
  };
  static char result[50];
  sprintf(result, "%.2d:%.2d:%.2d %.8s%3d %.14s %d\n",
          timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec,
          wday_name[timeptr->tm_wday], timeptr->tm_mday, mon_name[timeptr->tm_mon],
          1900 + timeptr->tm_year);
  return result;
}

int getch( void )
{
  char ch;
  int fd = fileno(stdin);
  struct termio old_tty, new_tty;

  ioctl(fd, TCGETA, &old_tty);
  new_tty = old_tty;
  new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
  ioctl(fd, TCSETA, &new_tty);
  fread(&ch, 1, sizeof(ch), stdin);
  ioctl(fd, TCSETA, &old_tty);

  return ch;
}

char *doiChuoi(char *s) {
  int i;
  for (i=0; i<strlen(s); i++)
    if (s[i] >= 65 && s[i]<=90)
      s[i] = s[i] + 32;
  return s;
}

int checkCharater(char *s) {
  int i;
  for (i=0; i<strlen(s); i++)
    if (s[i] < 48 || (s[i] > 57 && s[i] < 65) ||
        (s[i] > 90 && s[i] < 97 && s[i] != 95)|| (s[i] > 122))
      return 0;
  return 1;
}

int checkLength(char *s) {
  if (strlen(s) < 6 )
    return -1;
  if (strlen(s) > 30)
    return 1;
  return 0;
}

void getNewPassword(char newpass[], char oldpass[]) {
  char pass[50], newpass1[50], newpass2[50];

  while (getchar()!='\n');
  while (1) {
    printf("Nhap mat khau hien tai: ");
    hidePassword(pass);
    if (strcmp(pass, oldpass) != 0) {
      printf("Mat khau khong chinh xac\n");
      continue;
    }

    printf("Nhap mat khau moi: ");
    hidePassword(newpass1);
    if (checkLength(newpass1) < 0) {
      printf("Mat khau qua ngan\n");
      continue;
    }
    else
      if (checkLength(newpass1) > 0) {
        printf("Mat khau qua dai\n");
        continue;
      }

    if (checkCharater(newpass1) == 0) {
      printf("Mat khau chua ki tu khong hop le\n");
      continue;
    }

    printf("Nhap lai mat khau moi: ");
    hidePassword(newpass2);
    strcpy(newpass1, doiChuoi(newpass1));
    strcpy(newpass2, doiChuoi(newpass2));
    if (strcmp(newpass1, newpass2) == 0) {
      strcpy(newpass, newpass1);
      return;
    }
    else
      printf("Mat khau moi khong chinh xac\n");
  }
}

Account Login(void) {
  Account person;
  while (getchar()!='\n');
  printf("Ten dang nhap: ");
  gets(person.user);
  printf("Mat khau: ");
  hidePassword(person.pass);
  return person;
}

char *getUsername(void) {

  char temp[255];
  while (getchar() != '\n');
  for (;;) {

    printf("Ten dang nhap (6-30 ki tu): ");
    gets(temp);
    if (checkLength(temp) < 0) {
      printf("Ten dang nhap qua ngan\n");
      continue;
    }
    else
      if (checkLength(temp) > 0) {
        printf("Ten dang nhap qua dai\n");
        continue;
      }

    if (checkCharater(temp) == 0) {
      printf("Ten dang nhap chua ki tu khong hop le\n");
      continue;
    }

    strcpy(temp, doiChuoi(temp));
    return temp;
  }
}

void hidePassword(char temp[]) {
  int i = 0;
  char ch;

  for (;;) {
    ch = getch();
    if (ch == 10) {
      temp[i] = '\0';
      printf("\n");
      return;
    }
    if (ch == 127) {
      if (i > 0)
        temp[--i] = '\0';
      continue;
    }
    temp[i++] = ch;
  }
}

char *getPassword(void) {
  char temp[50], _temp[50];
  for (;;) {

    printf("Mat khau (6-30 ki tu): ");
    hidePassword(temp);
    if (checkLength(temp) < 0) {
      printf("Mat khau qua ngan\n");
      continue;
    }
    else
      if (checkLength(temp) > 0) {
        printf("Mat khau qua dai\n");
        continue;
      }

    if (checkCharater(temp) == 0) {
      printf("Mat khau chua ki tu khong hop le\n");
      continue;
    }

    printf("Nhap lai mat khau: ");
    hidePassword(_temp);

    strcpy(temp, doiChuoi(temp));
    strcpy(_temp, doiChuoi(_temp));
    if (strcmp(temp, _temp) == 0)
      return temp;
    else
      printf("Mat khau khong khop\n");
  }
}

int checkPhoneNumber(char *s) {
  int i;
  for (i=0; i<strlen(s); i++)
    if (s[i] < 48 || s[i] > 57)
      return 0;
  return 1;
}

void getInfor(char *s1, char *s2, char *s3) {
  printf("Nhap email: ");
  gets(s1);
  printf("Nhap dia chi: ");
  gets(s2);
  for (;;) {
    printf("Nhap so dien thoai: ");
    gets(s3);
    if (checkPhoneNumber(s3) == 0) {
      printf("So dien thoai khong hop le\n");
      continue;
    }
    return;
  }
}

Account Register(void) {
  Account person;
  strcpy(person.user, getUsername());
  strcpy(person.pass, getPassword());
  getInfor(person.email, person.addr, person.phone);
  person.onl = 0;
  person.lvl = 0;
  person.coin = 0;
  person.listid = NULL;
  return person;
}

void UserInterface(Account *person, int sockfd) {
  int n, key, k, price, length;
  Account _person;
  Product _product;
  boughtList *cur;
  char newpass[50], sendbuff[MAXLINE], recvbuff[MAXLINE];
  struct tm* timeinfo;

  while (1) {
    printf("**************USER**************************\n");
    printf("*      1. Cap nhat dau gia                 *\n");
    printf("*      2. Dat gia                          *\n");
    printf("*      3. Kiem tra tai khoan               *\n");
    printf("*      4. Danh sach san pham               *\n");
    printf("*      5. Danh sach san pham da mua        *\n");
    printf("*      6. Doi mat khau                     *\n");
    printf("*      7. Dang xuat                        *\n");
    printf("********************************************\n");
    printf("Chon: ");
    scanf("%d", &n);
    switch (n) {
    case 1:
      key = UPDATE;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k == -1) {
        length = read(sockfd, recvbuff, MAXLINE);
        recvbuff[length] = '\0';
        printf("%s", recvbuff);
        continue;
      }
      else
        if (k == 1) {
          read(sockfd, &_product, sizeof(Product));
          printf("Ten san pham: %s\n", _product.name);
          printf("Mo ta: %s\n", _product.descripton);
          printf("Gia khoi diem: %d\n", _product.price);
          printf("Gia cao nhat hien tai: %d\n", _product.max_price);
          if (strcmp(_product.master, "") != 0)
            printf("Nguoi dat gia cao nhat: %s\n", _product.master);
          timeinfo = localtime(&_product.finish);
          printf("Thoi gian ket thuc: %s", _asctime(timeinfo));
        }
        else {
          read(sockfd, &_product, sizeof(Product));
          length = read(sockfd, recvbuff, MAXLINE);
          recvbuff[length] = '\0';
          printf("%s", recvbuff);
          if (strcmp(_product.master, "") == 0) {
            printf("Ten san pham: %s\n", _product.name);
            printf("Khong co ai dat gia cho san pham nay\n");
          }
          else {
            printf("Ten san pham: %s\n", _product.name);
            printf("Nguoi chien thang: %s\n", _product.master);
            printf("Gia ban: %d\n", _product.max_price);
          }
        }
      break;
    case 2:
      key = UPDATE_ACCOUNT;
      write(sockfd, &key, sizeof(int));
      _person = *person;
      write(sockfd, &_person, sizeof(Account));
      read(sockfd, &_person, sizeof(Account));
      *person = _person;
      key = AUCTIONS;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k == 0) {
        printf("Hien tai khong co san pham nao duoc dau gia\n");
        continue;
      }
      k = 1;
      printf("Nhap gia ban muon dat: ");
      scanf("%d", &price);
      if (price > (*person).coin) {
        k = 0;
        write(sockfd, &k, sizeof(int));
        printf("Tai khoan cua ban khong du de dat gia\n");
        continue;
      }
      write(sockfd, &k, sizeof(int));
      write(sockfd, &price, sizeof(int));
      write(sockfd, (*person).user, strlen((*person).user));
      length = read(sockfd, recvbuff, MAXLINE);
      recvbuff[length] = '\0';
      printf("%s", recvbuff);
      break;
    case 3:
      key = UPDATE_ACCOUNT;
      write(sockfd, &key, sizeof(int));
      _person = *person;
      write(sockfd, &_person, sizeof(Account));
      read(sockfd, &_person, sizeof(Account));
      *person = _person;
      printf("Tai khoan cua ban la %d\n", (*person).coin);
      break;
    case 4:
      key = LIST_PRODUCT;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k==0) {
        printf("Chua co san pham nao da duoc dau gia\n");
        continue;
      }
      while (k > 0) {
        read(sockfd, &_product, sizeof(Product));
        printf("ID: %d\nTen san pham: %s\n", _product.id, _product.name);
        printf("Trang thai: ");
        if (_product.status == 1)
          printf("Dang dau gia\n");
        else
          printf("Da ket thuc\n");
        if (strcmp(_product.master, "") != 0 && (_product.status == 0)) {
          printf("Gia ban: %d\n", _product.max_price);
          printf("Nguoi thang cuoc: %s\n", _product.master);
        }
        k--;
      }
      break;

    case 5:
      key = UPDATE_ACCOUNT;
      write(sockfd, &key, sizeof(int));
      _person = *person;
      write(sockfd, &_person, sizeof(Account));
      read(sockfd, &_person, sizeof(Account));
      *person = _person;
      if ((*person).listid == NULL) {
        printf("Ban chua mua san pham nao\n");
        continue;
      }
      key = BOUGHT_PRODUCTS;
      write(sockfd, &key, sizeof(int));
      cur = (*person).listid;
      while (cur != NULL) {
        k = cur->id;
        write(sockfd, &k, sizeof(int));
        read(sockfd, &_product, sizeof(Product));
        printf("ID: %d\n", _product.id);
        printf("Ten san pham: %s\n", _product.name);
        printf("Gia mua: %d\n", _product.max_price);
        cur = cur->next;
      }
      k = 0;
      write(sockfd, &k, sizeof(int));
      break;

    case 6:
      key = CHANGE_PASSWORD;
      _person = *person;
      write(sockfd, &key, sizeof(int));
      getNewPassword(newpass, (*person).pass);
      write(sockfd, &_person, sizeof(Account));
      write(sockfd, newpass, strlen(newpass));
      read(sockfd, &k, sizeof(int));
      if (k == 1) {
        strcpy((*person).pass, newpass);
        printf("Doi mat khau thanh cong\n");
      }
      break;

    default:
      key = LOGOUT;
      write(sockfd, &key, sizeof(int));
      _person = *person;
      write(sockfd, &_person, sizeof(Account));
      return;
    }
  }
}

void ProductManage(int sockfd) {
  int n, key, k, flag, id, length;
  Product _product;
  char sendbuff[MAXLINE], recvbuff[MAXLINE];
  float h;
  struct tm* timeinfo;
  while (getchar() != '\n');
  while (1) {
    printf("********QUAN LY SAN PHAM************\n");
    printf("*    1. Hien thi danh sach         *\n");
    printf("*    2. Them san pham              *\n");
    printf("*    3. Chi tiet san pham          *\n");
    printf("*    4. Quay lai menu chinh        *\n");
    printf("************************************\n");
    printf("Chon: ");
    scanf("%d", &n);
    switch (n) {
    case 1:
      key = LIST_PRODUCT;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k==0) {
        printf("Chua co san pham nao\n");
        continue;
      }
      while (k > 0) {
        read(sockfd, &_product, sizeof(Product));
        printf("%d %s \n", _product.id, _product.name);
        k--;
      }
      break;

    case 2:
      key = ADD_PRODUCT;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k == 1) {
        printf("San pham khac dang duoc dau gia\n");
        continue;
      }
      while (getchar() != '\n');
      printf("Nhap ten san pham: ");
      gets(_product.name);
      printf("Nhap thong tin ve san pham: ");
      gets(_product.descripton);
      printf("Nhap gia khoi diem: ");
      scanf("%d", &_product.price);
      _product.max_price = _product.price;
      time(&_product.start);

      while (1) {
        flag = 0;
        printf("Nhap thoi gian dau gia (toi da 7 ngay):\n");
        printf("Chon don vi: 1. Giay\n");
        printf("             2. Phut\n");
        printf("             3. Gio\n");
        printf("             4. Ngay\n");
        printf("Chon don vi: ");
        scanf("%d", &k);
        printf("Nhap thoi gian: ");
        scanf("%f", &h);
        switch (k) {
        case 1:
          if (h > TIME_MAX || h < 0) {
            printf("Nam ngoai khoang thoi gian cho phep\n");
            continue;
          }
          _product.finish = _product.start + h;
          flag = 1;
          break;
        case 2:
          if (h*60 > TIME_MAX || h < 0) {
            printf("Nam ngoai khoang thoi gian cho phep\n");
            continue;
          }
          _product.finish = _product.start + h*60;
          flag = 1;
          break;
        case 3:
          if (h*3600 > TIME_MAX || h < 0) {
            printf("Nam ngoai khoang thoi gian cho phep\n");
            continue;
          }
          _product.finish = _product.start + h*3600;
          flag = 1;
          break;
        case 4:
          if (h*86400 > TIME_MAX || h < 0) {
            printf("Nam ngoai khoang thoi gian cho phep\n");
            continue;
          }
          _product.finish = _product.start + h*86400;
          flag = 1;
          break;
        }
        if (flag == 1) break;
      }
      strcpy(_product.master, "");
      write(sockfd, &_product, sizeof(Product));
      printf("San pham da duoc dua len dau gia\n");
      break;
    case 3:
      key = DETAIL_PRODUCT;
      write(sockfd, &key, sizeof(int));
      printf("Nhap id san pham: ");
      scanf("%d", &id);
      write(sockfd, &id, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k == 0) {
        length = read(sockfd, recvbuff, MAXLINE);
        recvbuff[length] = '\0';
        printf("%s", recvbuff);
        continue;
      }
      read(sockfd, &_product, sizeof(Product));
      printf("Ten san pham: %s\n", _product.name);
      printf("Mo ta: %s\n", _product.descripton);
      printf("Gia khoi diem: %d\n", _product.price);
      if (_product.status == 0 && strcmp(_product.master, "") != 0)
        printf("Gia ban: %d\n", _product.max_price);
      timeinfo = localtime(&_product.start);
      printf("Thoi gian bat dau: %s", _asctime(timeinfo));
      timeinfo = localtime(&_product.finish);
      printf("Thoi gian ket thuc: %s", _asctime(timeinfo));
      printf("Trang thai: ");
      if (_product.status == 1)
        printf("Dang dau gia\n");
      else {
        printf("Da ket thuc\n");
        if (strcmp(_product.master, "") != 0)
          printf("Chu so huu: %s\n", _product.master);
      }
      break;

    default:
      return;
    }
  }
}

void AccountManage(int sockfd) {
  int n, k, key, length;
  AccountList *acclist;
  Account person;
  char sendbuff[MAXLINE], recvbuff[MAXLINE], ch;
  while (getchar() != '\n');
  while (1) {
    printf("********QUAN LY TAI KHOAN***********\n");
    printf("*    1. Hien thi danh sach         *\n");
    printf("*    2. Xoa tai khoan              *\n");
    printf("*    3. Tang tien                  *\n");
    printf("*    4. Quay lai menu chinh        *\n");
    printf("************************************\n");
    printf("Chon: ");
    scanf("%d", &n);
    switch (n) {
    case 1:
      key = LIST_ACCOUNT;
      write(sockfd, &key, sizeof(int));
      read(sockfd, &k, sizeof(int));
      if (k == 0) {
        printf("Chua co tai khoan nao\n");
        continue;
      }
      while (k > 0) {
        read(sockfd, &person, sizeof(Account));
        printf("%s %s %s %s %d %d %d\n", person.user, person.email, person.addr,
               person.phone, person.onl, person.lvl, person.coin);
        k--;
      }
      break;

    case 2:
      key = DELETE_ACCOUNT;
      write(sockfd, &key, sizeof(int));
      while (getchar() != '\n');
      printf("Nhap ten tai khoan muon xoa: ");
      gets(sendbuff);
      write(sockfd, sendbuff, strlen(sendbuff));
      printf("Ban co chac chan muon xoa tai khoan %s: Y/N\n", sendbuff);
      ch = getchar();
      if (ch == 'N' || ch == 'n') {
        k = 0;
        write(sockfd, &k, sizeof(int));
        continue;
      }
      if (ch == 'Y' || ch == 'y') {
        k = 1;
        write(sockfd, &k, sizeof(int));
        length = read(sockfd, recvbuff, MAXLINE);
        recvbuff[length] = '\0';
        printf("%s", recvbuff);
      }
      break;

    case 3:
      key = SETCOIN;
      write(sockfd, &key, sizeof(int));
      while (getchar() != '\n');
      printf("Nhap ten tai khoan: ");
      gets(sendbuff);
      printf("Nhap so tien: ");
      scanf("%d", &k);
      write(sockfd, sendbuff, strlen(sendbuff));
      write(sockfd, &k, sizeof(int));
      length = read(sockfd, recvbuff, MAXLINE);
      recvbuff[length] = '\0';
      printf("%s", recvbuff);
      break;

    default:
      return;
    }
  }
}

void AdminInterface(Account *person, int sockfd) {
  int n, key, k;
  Account _person;
  char newpass[50];
  while (1) {
    printf("**************ADMIN*****************\n");
    printf("*     1. Quan ly tai khoan         *\n");
    printf("*     2. Quan ly san pham          *\n");
    printf("*     3. Doi mat khau              *\n");
    printf("*     4. Dang xuat                 *\n");
    printf("************************************\n");
    printf("Chon: ");
    scanf("%d", &n);
    switch (n) {
    case 1:
      AccountManage(sockfd);
      break;
    case 2:
      ProductManage(sockfd);
      break;
    case 3:
      key = CHANGE_PASSWORD;
      _person = *person;
      write(sockfd, &key, sizeof(int));
      getNewPassword(newpass, (*person).pass);
      write(sockfd, &_person, sizeof(Account));
      write(sockfd, newpass, strlen(newpass));
      read(sockfd, &k, sizeof(int));
      if (k == 1) {
        strcpy((*person).pass, newpass);
        printf("Doi mat khau thanh cong\n");
      }
      break;

    default:
      key = LOGOUT;
      write(sockfd, &key, sizeof(int));
      _person = *person;
      write(sockfd, &_person, sizeof(Account));
      return;
    }
  }
}

void menu(int sockfd) {
  int flag, n, length, key;
  char sendbuff[MAXLINE], recvbuff[MAXLINE];
  Account person, _person;
  while(1) {
    printf("************************************\n");
    printf("*          1. Dang ki              *\n");
    printf("*          2. Dang nhap            *\n");
    printf("*          3. Thoat                *\n");
    printf("************************************\n");
    printf("Chon: ");
    scanf("%d", &n);
    switch (n) {
    case 1:
      person = Register();
      key = REGISTER;
      write(sockfd, &key, sizeof(int));
      write(sockfd, &person, sizeof(Account));
      length = read(sockfd, recvbuff, MAXLINE);
      recvbuff[length] = '\0';
      printf("%s", recvbuff);
      break;
    case 2:
      person = Login();
      key = LOGIN;
      write(sockfd, &key, sizeof(int));
      write(sockfd, &person, sizeof(Account));

      read(sockfd, &flag, sizeof(int));
      read(sockfd, &_person, sizeof(Account));
      length = read(sockfd, recvbuff, MAXLINE);
      recvbuff[length] = '\0';
      printf("%s", recvbuff);

      if (flag != 1) continue;
      if (_person.lvl == -1)
        AdminInterface(&_person, sockfd);
      else
        UserInterface(&_person, sockfd);
      break;
    default:
      return;
    }
  }
}

void process(int sockfd)
{
  menu(sockfd);
}

int main(int argc, char **argv)
{
  int sock;
  struct sockaddr_in server_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  bzero(&(server_addr.sin_zero),8);

  if (connect(sock, (struct sockaddr *)&server_addr,
              sizeof(struct sockaddr)) == -1)
    {
      perror("Connect");
      exit(1);
    }

  process(sock);
  exit(0);
}
