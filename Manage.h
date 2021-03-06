#define TIME_MAX 604800
#define MAXLINE 255
#define REGISTER 1
#define LOGIN 2
#define LIST_ACCOUNT 3
#define DELETE_ACCOUNT 4
#define SETCOIN 5
#define CHANGE_PASSWORD 6
#define LOGOUT 7
#define LIST_PRODUCT 8
#define ADD_PRODUCT 9
#define DETAIL_PRODUCT 10
#define UPDATE 11
#define AUCTIONS 12
#define UPDATE_ACCOUNT 13
#define BOUGHT_PRODUCTS 14

struct Product_ {
  int id;
  char name[30];
  char descripton[100];
  int status;
  int price;
  int max_price;
  time_t start;
  time_t finish;
  char master[30];
};

typedef struct Product_ Product;

struct ProductList_ {
  Product product;
  struct ProductList_ *next;
};

typedef struct ProductList_ ProductList;

struct boughtList_ {
  int id;
  struct boughtList_ *next;
};

typedef struct boughtList_ boughtList;

struct Account_ {
  char user[30];
  char pass[30];
  char email[50];
  char addr[50];
  char phone[15];
  int onl;
  int lvl;
  int coin;
  boughtList *listid;
};

typedef struct Account_ Account;

struct AccountList_ {
  Account account;
  struct AccountList_ *next;
};

typedef struct AccountList_ AccountList;

struct Program_ {
  AccountList *accList;
  ProductList *proList;
};

typedef struct Program_ Program;
