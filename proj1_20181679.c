#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
	int nu; //회원번호
	char name[20]; //이름
	char id[20]; //아이디
	char password[20]; //비밀번호
	char address[100]; //주소
	int money; //가상계좌
	int status; //상태(탈퇴를 위함)
}person; //회원 구조체

typedef struct Product{
	int nu; //상품번호
	char name[40]; //상품이름
	char category[40]; //카테고리
	int price; //가격
	int status; //상태(등록 및 삭제를 위함)
	int rate; //판매량 알아보기 위함
}prod; //상품 구조체

typedef struct History{
	int nu; //구매번호
	int prnu; //상품번호
	int pnu; //회원번호
	int status; //배송상태(배송준비,배송중,배송완료)
}his; //구매내역 구조체

typedef struct _Node{
	person *p; 
	prod *pr;
	his *hs;
	struct _Node *link;
}Node; //링크드리스트 구현 위함

void user_register(Node*); //회원가입
int user_login(Node*, Node *,Node*); //회원로그인
int user_page(Node*, Node*,Node*,Node*); //회원 페이지
void create_Node(Node*); //노드 새로 만들기
int admin_login(Node*,Node*,Node*,Node*); //관리자 로그인
int admin_page(Node*,Node*,Node*,Node*); //관리자 페이지
void get_user_data(Node*); //customer.csv에서 회원정보 쭉 읽어오기
void get_admin_data(Node*); //admin.csv에서 관리자정보 쭉 읽어오기
void get_prod_data(Node*); //product.csv에서 상품정보 쭉 읽어오기
int user_data_edit(Node*,Node*); //회원정보 수정
int search_product(Node *); //상품 찾기
int buy_product(Node*,Node*,Node*,Node*); //상품 구매
int virtual_account(Node*,Node*); //가상계좌
int view_user_data(Node*); //회원정보조회
int prod_regist_del(Node*); //상품 등록 및 삭제
int prod_data_edit(Node*); //상품 정보 수정
int view_purchase_hist(Node*,Node*,Node*,Node*); //구매내역 조회
int view_delivery_hist(Node*,Node*,Node*,Node*); //배송 내역 조회(배송중인 상품만)
int view_prod_purchase_info(Node*,Node*); //상품 및 구매 내역 조회
int view_purchase_statistics(Node*,Node*); //상품 및 구매 내역 통계
int delivery_data_edit(Node*,Node*); //배송 내역 수정
void free_func(Node*,Node*,Node*,Node*);//리스트 전부 프리

void create_Node(Node *cur){

	Node *new;
	new=(Node*)malloc(sizeof(Node));
	new->p=(person*)malloc(sizeof(person));
	new->pr = (prod*)malloc(sizeof(prod));
	new->hs = (his*)malloc(sizeof(his));

	new->link=NULL;
	cur->link=new;
} //새로운 노드를 맨뒤에 추가해주는 식의 링크드 리스트를 만들기 위함

void get_user_data(Node *head){
	
	FILE* fp;
	Node* tmp;
	int i;
	char c;
	int line_nu=0;
	
	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp=head;

	fp=fopen("customer.csv","r");
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}
	fclose(fp);

	fp=fopen("customer.csv","r");
	fscanf(fp,"%*[^\n]\n");
	for(i=0;i<line_nu;i++){
		if(head->link==NULL){	
			fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d\n",&(head->p->nu),head->p->id,head->p->password,head->p->name,head->p->address,&(head->p->money),&(head->p->status));
			create_Node(head);
			head=head->link;
		}
	}
	
	head=tmp;
	
	fclose(fp);

} //파일에서 한줄 씩 읽을 때 마다 노드에 정보를 넣어주고 새로운 노드 생성하는 방식으로 링크드 리스트를 만들어준다.

void get_prod_data(Node *head){
	
	FILE* fp;
	Node* tmp;
	int i;
	char c;
	int line_nu=0;
	
	tmp=(Node*)malloc(sizeof(Node));
	tmp->pr=(prod*)malloc(sizeof(prod));
	tmp=head;

	fp=fopen("product.csv","r");
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}

	fclose(fp);
	fp=fopen("product.csv","r");
	fscanf(fp,"%*[^\n]\n");
	
	for(i=0;i<line_nu-1;i++){
		if(head->link==NULL){	
			fscanf(fp,"%d,%[^,],%[^,],%d,%d\n",&(head->pr->nu),head->pr->name,head->pr->category,&(head->pr->price),&(head->pr->status));
			create_Node(head);
			head=head->link;
		}
	}
	
	head=tmp;
	fclose(fp);

} //get user data와 같은 방식

void get_admin_data(Node *head){
	
	FILE* fp;
	Node* tmp;
	int i;
	char c;
	int line_nu=0;
	
	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp=head;

	fp=fopen("admin.csv","r");
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}
	fclose(fp);

	fp=fopen("admin.csv","r");
	fscanf(fp,"%*[^\n]\n");
	for(i=0;i<line_nu;i++){
		if(head->link==NULL){
			fscanf(fp,"%d,%[^,],%[^,],%[^\n]\n",&(head->p->nu),head->p->name,head->p->id,head->p->password);
			create_Node(head);
			head=head->link;
		}
	}
	
	head=tmp;

	fclose(fp);

} //위와 같음

void get_hist_data(Node *head){
	
	FILE* fp;
	Node* tmp;
	int i;
	char c;
	int line_nu=0;
	
	tmp=(Node*)malloc(sizeof(Node));
	tmp->hs=(his*)malloc(sizeof(his));
	tmp=head;

	fp=fopen("history.csv","r");
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}
	fclose(fp);

	fp=fopen("history.csv","r");
	fscanf(fp,"%*[^\n]\n");
	for(i=0;i<line_nu-1;i++){
		if(head->link==NULL){
			fscanf(fp,"%d,%d,%d,%d\n",&(head->hs->nu),&(head->hs->prnu),&(head->hs->pnu),&(head->hs->status));
			create_Node(head);
			head=head->link;
		}
	}
	
	head=tmp;

	fclose(fp);

} //위와 같음

void free_func(Node* head,Node* head_pr,Node* head_ad,Node* head_hs){
	
	Node* cur;
	Node* tmp;
	cur=(Node*)malloc(sizeof(Node));
	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp->pr=(prod*)malloc(sizeof(prod));
	tmp->hs=(his*)malloc(sizeof(his));
	cur->p=(person*)malloc(sizeof(person));
	cur->pr=(prod*)malloc(sizeof(prod));
	cur->hs=(his*)malloc(sizeof(his));

	cur=head;
	while(cur!=NULL){
		tmp=cur->link;
		free(cur->p);
		free(cur);
		cur=tmp;
	}
	cur=head_pr;
	while(cur!=NULL){
		tmp=cur->link;
		free(cur->pr);
		free(cur);
		cur=tmp;
	}
	cur=head_ad;
	while(cur!=NULL){
		tmp=cur->link;
		free(cur->p);
		free(cur);
		cur=tmp;
	}
	cur=head_hs;
	while(cur!=NULL){
		tmp=cur->link;
		free(cur->hs);
		free(cur);
		cur=tmp;
	}

}//free 함수

int main(){
	
	int d,tmp;
	char c;
	FILE *fp;
	Node *head;
	Node *head_ad;
	Node *head_pr;
	Node *head_hs;

	while(1){
	
	head_pr=(Node*)malloc(sizeof(Node));
	head_pr->pr=(prod*)malloc(sizeof(prod));
	head_pr->link=NULL;
	
	head_ad=(Node*)malloc(sizeof(Node));
	head_ad->p=(person*)malloc(sizeof(person));
	head_ad->link=NULL;

	head=(Node*)malloc(sizeof(Node));
	head->p=(person*)malloc(sizeof(person));
	head->link=NULL;

	head_hs=(Node*)malloc(sizeof(Node));
	head_hs->hs=(his*)malloc(sizeof(his));
	head_hs->link=NULL;
//node의 head를 생성해준뒤 
	get_user_data(head);
	get_prod_data(head_pr);
	get_admin_data(head_ad);
	get_hist_data(head_hs);
//리스트를 만든다.
	system("clear");
	printf("SG-MALL에 오신 것을 환영합니다.\n");
	printf("1. 회원 로그인\n");
	printf("2. 회원 가입\n");
	printf("3. 상품 검색\n");
	printf("4. 관리자 로그인\n");
	printf("5. 종료\n");
	printf("선택: ");
	scanf("%d",&d);

	if(d==1)
		user_login(head,head_pr,head_hs);
	else if(d==2)
		user_register(head);
	else if(d==3){
		tmp=search_product(head_pr);
		if(tmp==1)
			getchar();
	}
	else if(d==4)
		admin_login(head,head_pr,head_ad,head_hs);
	else if(d==5){
		free_func(head,head_pr,head_ad,head_hs);
		return 0;
	}
	else{
		printf("잘못된 입력입니다. 다시 시도해주십시오.\n");
		getchar();
		scanf("%c",&c);
	}

	}
	
//선택에 따라서 해당 페이지로 넘어감
}

int user_login(Node* head, Node *head_pr,Node* head_hs){
	
	FILE* fp;
	char *id;
	char *pw;
	int i,j;
	char c;
	int* nu;
	int tmp=1;
	Node* cur;

	cur=(Node*)malloc(sizeof(Node));
	cur->p=(person*)malloc(sizeof(person));
	cur=head;
	
	id=(char*)malloc(sizeof(char)*30);
	pw=(char*)malloc(sizeof(char)*30);

	system("clear");
	printf("회원 로그인 페이지\n");

	for(i=0;i<3;i++){
		printf("아이디: ");
		scanf("%s",id);
		printf("비밀번호: ");
		scanf("%s",pw);
		
		while(cur != NULL){
			if(strcmp(cur->p->id,id)==0 && strcmp(cur->p->password,pw)==0){
				if(cur->p->status==-1){
					printf("탈퇴한 회원입니다.\n");
					c=getchar();
					scanf("%c",&c);
					tmp=0;
					break;
				}
				else{
					tmp=user_page(cur,head_pr,head,head_hs);//id와 password가 회원정보와 일치할 경우 회원페이지로 넘어감 
					if(tmp==0)
						break;
				}
			}
			cur=cur->link;
		}
		if(tmp == 0)
			break;

		printf("일치하는 회원이 없습니다.\n");
		if(i==2){
			printf("회원 아이디 또는 비밀번호가 일치하지 않습니다.\n");
			printf("확인 후 다시 로그인을 시도하여 주십시오.\n");
			c=getchar();
			scanf("%c",&c);
		}
		cur=head;
	}//i는 3번까지 로그인 3회 시도가능

	return tmp;
}//회원 로그인 페이지

void user_register(Node* head){
	
	FILE* fp;
	char *repw;
	char *idcheck;
	int i,line_nu=0;
	char c;
	Node *user;
	Node *tmp;
	
	idcheck=(char*)malloc(sizeof(char)*50);
	repw=(char*)malloc(sizeof(char)*50);
	user=(Node*)malloc(sizeof(Node));
	user->p=(person*)malloc(sizeof(person));
	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));

	user=head;
	tmp=user;

	while(user->link!=NULL){
		user=user->link;
	}

	user->p->money=0;
	user->p->status=1;

	fp=fopen("customer.csv","a+");//파일을 열어서 맨 뒤에 작성
	
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}//회원번호를 입력하기 위함

	system("clear");
	printf("회원가입 페이지\n");
	fprintf(fp,"%d,",line_nu);
	printf("이름: ");
	scanf("%s",user->p->name);
	while(1){
		printf("아이디: ");
		scanf("%s",user->p->id);
		idcheck=user->p->id;
		
		user=head;
		while(user->link!=NULL){
			if(strcmp(user->p->id,idcheck)==0){
				printf("중복된 id가 있습니다.\n");//id는 다른 회원과 구별해야함
				break;
			}
			else user=user->link;
		}
		
		if(user->link==NULL)
			break;
		else{
			while(user->link!=NULL)
				user=user->link;
		}
	}
	
	while(1){
		printf("비밀번호: ");
		scanf("%s",user->p->password);
	
		printf("비밀번호 확인: ");
		scanf("%s",repw);
		//비밀번호와 비밀번호 확인이 일치할 경우 다음으로 넘어감
	
		if(strcmp(user->p->password,repw)==0){
			fprintf(fp,"%s,",user->p->id);
			fprintf(fp,"%s,",user->p->password);
			fprintf(fp,"%s,",user->p->name);
			printf("주소: ");
			scanf(" %[^\n]",user->p->address);
			fprintf(fp,"%s,",user->p->address);
			fprintf(fp,"%d,",user->p->money);
			fprintf(fp,"%d\n",user->p->status);
			break;
		}
		else{
			printf("비밀번호가 일치하지 않습니다.\n");
		}
	}
	
	create_Node(user);
	head=tmp;

	fclose(fp);

}//회원 가입 페이지

int user_page(Node* cur,Node *head_pr,Node* head,Node* head_hs){

	int d;
	int tmp;
	char c;
	tmp=1;
	while(1){

	get_user_data(head);
	get_prod_data(head_pr);
	get_hist_data(head_hs);
	system("clear");
//로그인에 성공했을 시 그 회원정보를 cur에 담고 들어옴
	printf("%s님, SG-MALL에 오신 것을 환영합니다.\n",cur->p->name);		
	printf("1. 회원 정보 수정\n");
	printf("2. 가상계좌 입금 및 출금\n");
	printf("3. 상품 검색 및 구매\n");
	printf("4. 구매 내역 조회\n");
	printf("5. 배송 내역 조회(배송 중)\n");
	printf("6. 로그아웃 및 종료\n");
	printf("선택 : ");
	scanf("%d",&d);	
	if(d==1)
		user_data_edit(cur,head);
	else if(d==2)
		virtual_account(cur,head);
	else if(d==3){
		tmp=search_product(head_pr);//상품검색
		if(tmp==1)
			buy_product(head_pr,cur,head,head_hs);//상품 구매
	}
	else if(d==4)
		view_purchase_hist(cur,head,head_pr,head_hs);
	else if(d==5)
		view_delivery_hist(cur,head,head_pr,head_hs);
	else if(d==6){
		break;
	}
	else{
		printf("올바른 입력이 아닙니다. 다시 시도해주십시오.\n");
		scanf("%c",&c);
		getchar();
	}
	
	}
	return 0;
}

int view_delivery_hist(Node* cur,Node* head,Node* head_pr,Node* head_hs){
	
	Node* tmp;//person
	Node* tmp2;//history
	Node* tmp3;//product

	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->hs=(his*)malloc(sizeof(his));
	tmp3=(Node*)malloc(sizeof(Node));
	tmp3->pr=(prod*)malloc(sizeof(prod));

	tmp=head;
	tmp2=head_hs;
	tmp3=head_pr;

	while(tmp->link!=NULL){
		if(strcmp(cur->p->id,tmp->p->id)==0){
			break;
		}
		else tmp=tmp->link;
	}
//현재 cur에 담긴 회원을 전체 회원에서 찾음
	while(tmp2->link!=NULL){
		if(tmp2->hs->pnu==tmp->p->nu){
			while(tmp3->link!=NULL){
				if(tmp3->pr->nu==tmp2->hs->prnu)
					break;
				else tmp3=tmp3->link;
			}
			if(tmp2->hs->status==1){
				printf("===================\n");
				printf("상품명: %s\n",tmp3->pr->name);
				printf("상품 번호: %d\n",tmp2->hs->prnu);
			}
		}
		tmp2=tmp2->link;
	}
//구매내역 리스트에서 회원정보가 일치하는 정보를 찾음
//그중 배송중인 상품만 printf해줌

	getchar();
	printf("===================\n");
	printf("ENTER키를 누르십시오.\n");
	getchar();
			
	return 0;
}//배송 내역 조회(배송중인 상품)


int view_purchase_hist(Node* cur,Node* head,Node* head_pr,Node* head_hs){
	
	Node* tmp;//person
	Node* tmp2;//history
	Node* tmp3;//product
	char* status;

	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->hs=(his*)malloc(sizeof(his));
	tmp3=(Node*)malloc(sizeof(Node));
	tmp3->pr=(prod*)malloc(sizeof(prod));
	status=(char*)malloc(sizeof(char)*100);

	tmp=head;
	tmp2=head_hs;
	tmp3=head_pr;

	while(tmp->link!=NULL){
		if(strcmp(cur->p->id,tmp->p->id)==0){
			break;
		}
		tmp=tmp->link;
	}
//현재 로그인된 cur를 전체 회원에서 찾음
	while(tmp2->link!=NULL){
		if(tmp2->hs->pnu==tmp->p->nu){
			while(tmp3->link!=NULL){
				if(tmp2->hs->prnu==tmp3->pr->nu)
					break;
				tmp3=tmp3->link;
			}
			printf("===================\n");
			printf("상품명: %s\n",tmp3->pr->name);
			printf("상품 번호: %d\n",tmp2->hs->prnu);
			if(tmp2->hs->status==0)
				strcpy(status,"배송 준비");
			else if(tmp2->hs->status==1)
				strcpy(status,"배송중");
			else if(tmp2->hs->status==2)
				strcpy(status,"배송 완료");
			printf("배송 상태: %s\n",status);

		}
		tmp2=tmp2->link;
	}
	//배송내역에서 회원번호가 같은 내역을 전부찾고 그에 해당하는 상품번호를 가지고 상품정보 출력
	//상품명,상품번호,배송상태가 뜨도록 한다.
	//모든 구매 내역을 조회할 수 있다.
	//배송상태는 배송준비,배송중, 배송완료로 나뉨

	getchar();
	printf("===================\n");
	printf("ENTER키를 누르십시오.\n");
	getchar();

	return 0;

}//구매 내역 조회

int virtual_account(Node* cur,Node* head){
	
	FILE* fp;
	int d,money;
	char c;
	Node* tmp;

	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp=head;

	while(tmp->link!=NULL){
		if(strcmp(tmp->p->id,cur->p->id)==0)
			break;
		tmp=tmp->link;
	}
//cur에 해당하는 회원 전체 회원에서 찾음
	printf("==============================\n");
	printf("가상계좌 입금 및 출금\n");
	printf("내 가상계좌 잔액 : %d원\n",tmp->p->money);
	printf("==============================\n");
	printf("1.입금하기\n");
	printf("2.출금하기\n");
	printf("3.이전 페이지로 돌아가기\n");
	printf("4.선택 : ");
	scanf("%d",&d);
	if(d==1){
		printf("===========================\n");
		printf("입금금액 입력 : ");
		scanf("%d",&money);
		getchar();
		tmp->p->money+=money;
		printf("입금이 완료되었습니다\n");
		printf("현재 잔액: %d원\n",tmp->p->money);
	}
//해당 node의 가상계좌에 돈을 입금한다.
	else if(d==2){
		if(tmp->p->money==0){
			printf("잔액이 부족합니다. 입금 후 사용해주십시오.\n");
			getchar();
		}//돈이 0이면 출금할수 없다.
		else{
			printf("출금금액 입력 : ");
			scanf("%d",&money);
			getchar();
			if(tmp->p->money<money){
				printf("잔액이 부족합니다. 입금 후 사용해주십시오.\n");
				getchar();
			}//잔액이 부족하면 출금할 수 없다.
			else{
				tmp->p->money-=money;
				printf("출금이 완료되었습니다\n");
				printf("현재 잔액: %d원\n",tmp->p->money);
			}
		}
	}
	//해당 node의 가상계좌에서 돈을 출금한다.
	else if(d==3)
		return 0;

	fp=fopen("customer.csv","w");
	fprintf(fp,"회원정보,아이디,비밀번호,이름,주소,잔액,상태\n");
	tmp=head;
	while(tmp->link->link!=NULL){
		fprintf(fp,"%d,%s,%s,%s,%s,%d,%d\n",tmp->p->nu,tmp->p->id,tmp->p->password,tmp->p->name,tmp->p->address,tmp->p->money,tmp->p->status);
		tmp=tmp->link;
	}

	fclose(fp);
//customer.csv를 처음부터 다시 써준다. 이렇게 하면 바뀐 정보가 다시 들어가게됨.
	printf("ENTER키를 누르십시오.\n");
	scanf("%c",&c);

	return 0;
}//가상계좌 관련

int search_product(Node *head_pr){

	int d, tmp=0;
	char *name = (char *)malloc(sizeof(char)*100);
	char *category =(char*)malloc(sizeof(char)*100);

	Node *cur;
	cur=(Node*)malloc(sizeof(Node));
	cur->pr=(prod*)malloc(sizeof(prod));
	cur = head_pr;

	system("clear");
	printf("상품 검색 하기\n");
	printf("1. 이름으로 검색하기\n");
	printf("2. 카테고리로 검색하기\n");
	printf("---------------------\n");
	printf("0. 이전 화면으로 돌아가기\n");

	printf("선택: ");
	scanf("%d",&d);
	getchar();

	if(d==0)
		return 0;
	else if(d == 1)
	{
		printf("이름 검색 : ");
		scanf("%[^\n]",name);
		getchar();
		while(cur != NULL)
		{
			if(strcmp(name,cur->pr->name) == 0){
				printf("==============================\n");
				printf("상품코드: %d\n",cur->pr->nu);
				printf("상품이름 : %s\n",cur->pr->name);
				printf("카테고리 : %s\n",cur->pr->category);
				printf("가격 : %d\n",cur->pr->price);
				break;
			}
			cur = cur -> link;
		}
		if(cur==NULL){
			printf("상품이 존재하지 않습니다.\n");
			printf("ENTER키를 누르십시오.\n");
			getchar();
			return 0;
		}
		printf("==============================\n");
	
	}
	//이름으로 검색
	else if(d==2){
		printf("카테고리 검색 : ");
		scanf("%[^\n]",category);
		getchar();
		while(cur != NULL)
		{
			if(strcmp(category,cur->pr->category) == 0){
				printf("==============================\n");
				printf("상품코드: %d\n",cur->pr->nu);
				printf("상품이름: %s\n",cur->pr->name);
				printf("카테고리: %s\n",cur->pr->category);
				printf("가격: %d원\n",cur->pr->price);	
				tmp = 1;
			}
			cur = cur->link;
		}
		printf("==============================\n");
		printf("Press Enter");
		while(getchar() != '\n');
		return tmp;
	
	}
	//카테고리로 검색
	return 1;
}//상품 검색

int buy_product(Node* head_pr,Node* cur,Node* head,Node* head_hs){
	
	FILE* fp;
	int d1,d2,d3;
	int money,line_nu=0;
	char c;
	Node* tmp;//product
	Node* tmp2;//customer 정보
	Node* tmp3;//history용
	Node* tmp4;//history head 넣어둘 곳

	tmp=(Node*)malloc(sizeof(Node));
	tmp->pr=(prod*)malloc(sizeof(prod));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->p=(person*)malloc(sizeof(person));
	tmp3=(Node*)malloc(sizeof(Node));
	tmp3->hs=(his*)malloc(sizeof(his));
	tmp4=(Node*)malloc(sizeof(Node));
	tmp4->hs=(his*)malloc(sizeof(his));

	tmp4=head_hs;

	printf("==============================\n");
	printf("구매 할 상품 코드 입력 : ");
	scanf("%d",&d1);

	tmp=head_pr;
	while(tmp!=NULL){
		if(d1==tmp->pr->nu)
			break;
		tmp=tmp->link;
	}
	//입력한 상품코드와 일치하는 상품을 전체상품에서 찾아준다.

	printf("==============================\n");
	printf("상품코드: %d\n",tmp->pr->nu);
	printf("상품이름: %s\n",tmp->pr->name);
	printf("카테고리: %s\n",tmp->pr->category);
	printf("가격: %d원\n",tmp->pr->price);
	printf("==============================\n");
	printf("1. 구매하기\n");
	printf("2. 이전 페이지로 돌아가기\n");
	printf("선택 : ");
	scanf("%d",&d2);
	tmp2=head;
	tmp3=head_hs;

	if(d2==1){	
		while(tmp2!=NULL){
			if(strcmp(tmp2->p->id,cur->p->id)==0)
				break;
			tmp2=tmp2->link;
		}

		if(tmp->pr->status==-1){
			getchar();
			printf("삭제된 상품입니다.\n");
			printf("ENTER키를 누르십시오.\n");
			getchar();
		}//상품이 삭제 되었을 경우 상품 상태가 -1이므로 삭제된 상품은 구매를 할 수 없다.

		else{
		
		while(tmp3->link!=NULL){
			tmp3=tmp3->link;
		}
		
		tmp3->hs->prnu=tmp->pr->nu;
		tmp3->hs->pnu=tmp2->p->nu;
		tmp3->hs->status=0;
		create_Node(tmp3);

		printf("==============================\n");
		printf("내 가상계좌 잔액: %d\n",tmp2->p->money);
		printf("상품 가격: %d\n",tmp->pr->price);

		if((tmp->pr->price)>(tmp2->p->money)){
			printf("필요금액: %d\n",(tmp->pr->price)-(tmp2->p->money));
			printf("==============================\n");
			printf("1.가상계좌 입금하고 구매하기\n");
			printf("2.이전 페이지로 돌아가기\n");
			printf("선택: ");
			scanf("%d",&d3);
			if(d3==1){
				printf("입금금액 입력 : ");
				scanf("%d",&money);
				(tmp2->p->money)+=money;
				printf("현재 잔액: %d원\n",tmp2->p->money);
				printf("==============================\n");
				printf("구매가 완료되었습니다\n");
				(tmp2->p->money)-=(tmp->pr->price);
				printf("구매 후 잔액: %d원\n",tmp2->p->money);
				printf("ENTER키를 누르십시오.\n");
				c=getchar();
				scanf("%c",&c);

			}
			else if(d3==2)
				return 0;
		} //상품가격이 가상계좌 잔액보다 큰 경우 필요금액과 추가 입금을 해주고 구매를 진행한다.
		else{
			printf("구매가 완료되었습니다\n");
			(tmp2->p->money)-=(tmp->pr->price);
			printf("구매 후 잔액: %d원\n",tmp2->p->money);
			printf("ENTER키를 누르십시오.\n");
			c=getchar();
			scanf("%c",&c);
		}//구매를 완료하고 구매 후 잔액을 보여준다.

		}
	}
	else if(d2==2)
		return 0;

	fp=fopen("customer.csv","w");
	fprintf(fp,"회원정보,아이디,비밀번호,이름,주소,잔액,상태\n");
	tmp2=head;
	while(tmp2->link->link!=NULL){
		fprintf(fp,"%d,%s,%s,%s,%s,%d,%d\n",tmp2->p->nu,tmp2->p->id,tmp2->p->password,tmp2->p->name,tmp2->p->address,tmp2->p->money,tmp2->p->status);
		tmp2=tmp2->link;
	}

	fclose(fp);
	//잔액 상태가 바뀌었으므로 customer.csv를 새로 작성해준다.

	fp=fopen("history.csv","a+");
	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}
	tmp3->hs->nu=line_nu;
	fprintf(fp,"%d,%d,%d,%d\n",tmp3->hs->nu,tmp3->hs->prnu,tmp3->hs->pnu,tmp3->hs->status);
	fclose(fp);

	//구매내역에 새로운 정보를 입력해준다.
	return 0;

}//상품 구매

int user_data_edit(Node* cur,Node* head){
	
	FILE* fp;
	char* name;
	char* id;
	char* pw;
	char* address;
	int d;
	char c;
	Node* tmp;
	Node* tmp2;//아이디 중복 체크를 위함

	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->p=(person*)malloc(sizeof(person));
	tmp=(Node*)malloc(sizeof(Node));
	tmp->p=(person*)malloc(sizeof(person));
	tmp=head;

	while(tmp->link!=NULL){
		if(strcmp(tmp->p->id,cur->p->id)==0){
			break;
		}
		tmp=tmp->link;
	}

	name=(char*)malloc(sizeof(char)*50);
	id=(char*)malloc(sizeof(char)*50);
	pw=(char*)malloc(sizeof(char)*50);
	address=(char*)malloc(sizeof(char)*100);

	system("clear");
	printf("회원 정보 수정 페이지\n");
	printf("1. 이름 수정\n");
	printf("2. 아이디 수정\n");
	printf("3. 비밀번호 수정\n");
	printf("4. 주소 수정\n");
	printf("5. 회원 탈퇴\n");
	printf("선택 : ");
	scanf("%d",&d);

	printf("=======================\n");

	if(d==1){
		printf("이름: ");
		scanf("%s",name);
		strcpy(tmp->p->name,name);
	}
	else if(d==2){
		while(1){
		printf("아이디: ");
		scanf("%s",id);
		tmp2=head;
		while(tmp2->link!=NULL){
			if(strcmp(tmp2->p->id,id)==0){
				printf("중복된 아이디가 있습니다.\n");
				getchar();
				break;
			}
			else tmp2=tmp2->link;
		}//아이디는 이미 있는 아이디로 수정할 수 없음.
		if(tmp2->link==NULL)
			break;
		else{
			while(tmp2->link!=NULL){
				tmp2=tmp2->link;
			}
		}

		}
		strcpy(tmp->p->id,id);
	}
	else if(d==3){
		printf("비밀번호: ");
		scanf("%s",pw);
		strcpy(tmp->p->password,pw);
	}
	else if(d==4){
		printf("주소: ");
		scanf(" %[^\n]",address);
		strcpy(tmp->p->address,address);
	}
	else if(d==5){
		printf("탈퇴하시겠습니까? (y/n) : ");
		scanf(" %c",&c);
		if(c=='y'){
			tmp->p->status=-1;
		}
		else if(c=='n'){
			tmp->p->status=1;
		}
	}

	getchar();

	fp=fopen("customer.csv","w");
	fprintf(fp,"회원정보,아이디,비밀번호,이름,주소,잔액,상태\n");
	tmp=head;
	while(tmp->link->link!=NULL){
		fprintf(fp,"%d,%s,%s,%s,%s,%d,%d\n",tmp->p->nu,tmp->p->id,tmp->p->password,tmp->p->name,tmp->p->address,tmp->p->money,tmp->p->status);
		tmp=tmp->link;
	}

	fclose(fp);
	//회원정보가 바뀌었으므로 customer.csv를 새로 작성해준다.
	printf("=======================\n");
	printf("변경이 완료되었습니다.\n");
	printf("ENTER키를 누르십시오.\n");
	getchar();

	return 0;

}//회원정보 수정

int admin_login(Node* head,Node* head_pr,Node* head_ad,Node* head_hs){
	
	FILE* fp;
	Node* admin;
	char* id;
	char* pw;
	char c;
	int i,tmp;
	tmp=1;

	id=(char*)malloc(sizeof(char)*50);
	pw=(char*)malloc(sizeof(char)*50);

	admin=(Node*)malloc(sizeof(Node));
	admin->p=(person*)malloc(sizeof(person));
	
	admin=head_ad;
	
	system("clear");

	printf("관리자 로그인 페이지\n");
	for(i=0;i<3;i++){
		printf("아이디 : ");
		scanf("%s",id);
		printf("비밀번호 : ");
		scanf("%s",pw);

		while(admin!=NULL){
			if(strcmp(id,admin->p->id)==0&&strcmp(pw,admin->p->password)==0){
				system("clear");
				tmp=admin_page(head,head_pr,admin,head_hs);//id와 password가 일치할 경우 관리자 페이지로 넘어감.
				if(tmp == 0)
					break;
			}
			else
				admin=admin->link;
		}
		if(tmp == 0)
			break;

		printf("관리자 아이디 또는 비밀번호가 일치하지 않습니다.\n");
		if(i==2){
			printf("확인 후 다시 로그인을 시도하여 주세요.\n");
			c=getchar();
			scanf("%c",&c);
		}
		admin=head_ad;
	}//로그인은 3회까지 시도가능
	
	return tmp;
}//관리자 로그인

int admin_page(Node* head,Node* head_pr,Node* head_ad,Node* head_hs){
	
	int d;

	while(1){
	
	get_prod_data(head_pr);
	
	system("clear");
	printf("관리자 이름 : %s\n",head_ad->p->name);
	printf("SG-MALL 관리자 페이지\n");
	printf("1. 회원 정보 조회\n");
	printf("2. 상품 등록 및 삭제\n");
	printf("3. 상품 정보 수정\n");
	printf("4. 상품 정보 및 구매 내역 조회\n");
	printf("5. 상품 정보 및 구매 내역 통계\n");
	printf("6. 배송 내역 수정\n");
	printf("7. 로그아웃 및 종료\n");
	printf("선택 : ");
	scanf("%d",&d);
	getchar();
	
	get_user_data(head);
	get_prod_data(head_pr);
	get_hist_data(head_hs);

	if(d==1)
		view_user_data(head);//회원 정보 조회
	else if(d==2)
		prod_regist_del(head_pr);//상품 등록 및 삭제
	else if(d==3)
		prod_data_edit(head_pr);//상품 정보 수정
	else if(d==4)
		view_prod_purchase_info(head_pr,head_hs);//상품 정보 조회 및 구매 내역 조회
	else if(d==5)
		view_purchase_statistics(head_pr,head_hs);//상품 정보 통계 및 구매 내역 통계
	else if(d==6)
		delivery_data_edit(head_hs,head);//배송 내역 수정
	else if(d==7){
		break;
	}

	continue;

	}
	return 0;
}

int delivery_data_edit(Node* head_hs,Node* head){

	FILE* fp;
	int d;
	char* status;
	Node* tmp;//history
	Node* tmp2;//head
	tmp=(Node*)malloc(sizeof(Node));
	tmp->hs=(his*)malloc(sizeof(his));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->p=(person*)malloc(sizeof(person));
	status=(char*)malloc(sizeof(char)*100);
	tmp=head_hs;
	tmp2=head;

	system("clear");
	printf("배송 내역 수정 페이지\n");
	printf("===========================\n");
	printf("수정할 주문 번호 입력: ");
	scanf("%d",&d);
	//수정할 주문 번호를 입력받음.
	while(tmp->link!=NULL){
		if(tmp->hs->nu==d){
			break;
		}
		tmp=tmp->link;
	}
	//주문번호와 일치하는 배송내역을 발견 
	printf("===========================\n");
	printf("주문 번호: %d\n",tmp->hs->nu);
	printf("상품 번호: %d\n",tmp->hs->prnu);
	printf("회원 번호: %d\n",tmp->hs->pnu);

	while(tmp2->link!=NULL){
		if(tmp2->p->nu==tmp->hs->pnu){
			break;
		}
		tmp2=tmp2->link;
	}

	printf("아이디: %s\n",tmp2->p->id);
	printf("주소: %s\n",tmp2->p->address);
	if(tmp->hs->status==0)
		strcpy(status,"배송 준비");
	else if(tmp->hs->status==1)
		strcpy(status,"배송 중");
	else if(tmp->hs->status==2)
		strcpy(status,"배송 완료");
	printf("배송 현황: %s\n",status);

	printf("===========================\n");
	//배송내역에 대한 정보를 알려줌

	printf("배송 현황 변경\n");
	printf("1. 배송 준비\n");
	printf("2. 배송 중\n");
	printf("3. 배송 완료\n");
	printf("선택 : ");
	scanf("%d",&d);
	//배송 현황을 변경하기 위함
	if(d==1)
		tmp->hs->status=0;
	else if(d==2)
		tmp->hs->status=1;
	else if(d==3)
		tmp->hs->status=2;

	tmp=head_hs;
	fp=fopen("history.csv","w");
	fprintf(fp,"주문번호,상품번호,회원번호,배송현황\n");
	while(tmp->link!=NULL){
		fprintf(fp,"%d,%d,%d,%d\n",tmp->hs->nu,tmp->hs->prnu,tmp->hs->pnu,tmp->hs->status);
		tmp=tmp->link;
	}
	fclose(fp);
	//배송내역이 수정되었으므로 history.csv를 새로 작성해줌.
	return 0;
}//배송 내역 수정

int view_purchase_statistics(Node* head_pr,Node* head_hs){
	
	FILE* fp;
	int i,j,line_nu=0;
	Node* tmp;//history
	Node* tmp2;//product
	Node* tmp3;//sort용

	tmp=(Node*)malloc(sizeof(Node));
	tmp->hs=(his*)malloc(sizeof(his));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->pr=(prod*)malloc(sizeof(prod));
	tmp3=(Node*)malloc(sizeof(Node));
	tmp3->pr=(prod*)malloc(sizeof(prod));

	tmp=head_hs;
	tmp2=head_pr;
	while(tmp2->link!=NULL){
		tmp2->pr->rate=0;
		tmp2=tmp2->link;
		line_nu++;
	}
	tmp2=head_pr;

	system("clear");
	printf("==================================== 구매 내역 통계 =================================\n");
	
	tmp=head_hs;
	tmp2=head_pr;
	while(tmp->link!=NULL){
		while(tmp2->link!=NULL){
			if(tmp->hs->prnu==tmp2->pr->nu){
				(tmp2->pr->rate)++;
			}
			tmp2=tmp2->link;
		}
		tmp2=head_pr;
		tmp=tmp->link;
	}
	//구매내역리스트를 읽으면서 상품번호가 일치할때마다 해당 상품의 판매량을 1씩 증가시켜줌
	tmp2 = head_pr;
	for(i=0;i<line_nu;){
		while(tmp2->link!=NULL){
			if((tmp2->pr->rate)<(tmp2->link->pr->rate)){
				tmp3->pr->rate=tmp2->link->pr->rate;
				tmp2->link->pr->rate=tmp2->pr->rate;
				tmp2->pr->rate=tmp3->pr->rate;
				
				strcpy(tmp3->pr->name,tmp2->link->pr->name);
				strcpy(tmp2->link->pr->name,tmp2->pr->name);
				strcpy(tmp2->pr->name,tmp3->pr->name);
				
				tmp3->pr->nu=tmp2->link->pr->nu;
				tmp2->link->pr->nu=tmp2->pr->nu;
				tmp2->pr->nu=tmp3->pr->nu;
				
				tmp3->pr->price=tmp2->link->pr->price;
				tmp2->link->pr->price=tmp2->pr->price;
				tmp2->pr->price=tmp3->pr->price;

			}
			tmp2=tmp2->link;
		}
		tmp2=head_pr;
		i++;
	}
	//판매량 순대로 정렬 시켜줌
	tmp2=head_pr;
	while(tmp2->link!=NULL){
		printf("상품번호: %3d\t 상품명: %-22s\t 판매량:%3d\t 가격:%15d\n",tmp2->pr->nu,tmp2->pr->name,tmp2->pr->rate,tmp2->pr->price);
		tmp2=tmp2->link;
	}
	//통계 보여줌
	tmp2 = head_pr;
	for(i=0;i<line_nu;){
		while(tmp2->link!=NULL){
			if((tmp2->pr->nu)>(tmp2->link->pr->nu)){
				tmp3->pr->rate=tmp2->link->pr->rate;
				tmp2->link->pr->rate=tmp2->pr->rate;
				tmp2->pr->rate=tmp3->pr->rate;
				
				strcpy(tmp3->pr->name,tmp2->link->pr->name);
				strcpy(tmp2->link->pr->name,tmp2->pr->name);
				strcpy(tmp2->pr->name,tmp3->pr->name);
				
				tmp3->pr->nu=tmp2->link->pr->nu;
				tmp2->link->pr->nu=tmp2->pr->nu;
				tmp2->pr->nu=tmp3->pr->nu;
				
				tmp3->pr->price=tmp2->link->pr->price;
				tmp2->link->pr->price=tmp2->pr->price;
				tmp2->pr->price=tmp3->pr->price;

			}
			tmp2=tmp2->link;
		}
		tmp2=head_pr;
		i++;
	}
	//다시 원상복귀 시킴(상품 번호 순대로 다시 정렬)
	printf("=====================================================================================\n");
	printf("\n");
	printf("==================================== 상품 정보 통계 =================================\n");

	tmp2 = head_pr;
	for(i=0;i<line_nu;){
		while(tmp2->link!=NULL){
			if((tmp2->pr->price)<(tmp2->link->pr->price)){	
				strcpy(tmp3->pr->name,tmp2->link->pr->name);
				strcpy(tmp2->link->pr->name,tmp2->pr->name);
				strcpy(tmp2->pr->name,tmp3->pr->name);
				
				tmp3->pr->nu=tmp2->link->pr->nu;
				tmp2->link->pr->nu=tmp2->pr->nu;
				tmp2->pr->nu=tmp3->pr->nu;
				
				tmp3->pr->price=tmp2->link->pr->price;
				tmp2->link->pr->price=tmp2->pr->price;
				tmp2->pr->price=tmp3->pr->price;

			}
			tmp2=tmp2->link;
		}
		tmp2=head_pr;
		i++;
	}
	//상품 정보만 통계를 보여줄 것이므로 가격 순대로 정렬 시켜줌.
	tmp2=head_pr;
	while(tmp2->link!=NULL){
		printf("상품번호: %3d\t 상품명: %-22s\t 가격:%15d\n",tmp2->pr->nu,tmp2->pr->name,tmp2->pr->price);
		tmp2=tmp2->link;
	}
	//통계 보여줌
	tmp2 = head_pr;
	for(i=0;i<line_nu;){
		while(tmp2->link!=NULL){
			if((tmp2->pr->nu)>(tmp2->link->pr->nu)){

				strcpy(tmp3->pr->name,tmp2->link->pr->name);
				strcpy(tmp2->link->pr->name,tmp2->pr->name);
				strcpy(tmp2->pr->name,tmp3->pr->name);
				
				tmp3->pr->nu=tmp2->link->pr->nu;
				tmp2->link->pr->nu=tmp2->pr->nu;
				tmp2->pr->nu=tmp3->pr->nu;
				
				tmp3->pr->price=tmp2->link->pr->price;
				tmp2->link->pr->price=tmp2->pr->price;
				tmp2->pr->price=tmp3->pr->price;

			}
			tmp2=tmp2->link;
		}
		tmp2=head_pr;
		i++;
	}

	tmp2=head_pr;
	//다시 원상복귀 시켜줌(상품번호 순 대로 다시 정렬)
	printf("=====================================================================================\n");
	printf("ENTER키를 누르십시오.\n");

	getchar();

	return 0;
}//상품 정보 통계 및 구매 내역 통계


int view_prod_purchase_info(Node* head_pr,Node* head_hs){
	
	Node* tmp;
	Node* tmp2;
	tmp=(Node*)malloc(sizeof(Node));
	tmp->pr=(prod*)malloc(sizeof(prod));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->hs=(his*)malloc(sizeof(his));

	tmp=head_pr;
	tmp2=head_hs;
	system("clear");
	printf("상품 정보 및 구매 내역 조회 페이지\n");
	printf("============ 상품 정보 ============\n");
	printf("상품번호,상품명,카테고리,가격,상태\n");

	while(head_pr->link!=NULL){
		printf("%d,%s,%s,%d,%d\n",head_pr->pr->nu,head_pr->pr->name,head_pr->pr->category,head_pr->pr->price,head_pr->pr->status);
		head_pr = head_pr->link;
	}
	head_pr=tmp;
	//상품 리스트 쭉 읽어서 정보 보여줌.
	
	printf("===================================\n");
	printf("\n");
	printf("============ 구매 내역 ============\n");
	printf("주문번호,상품번호,회원번호,배송현황\n");

	while(head_hs->link!=NULL){
		printf("%d,%d,%d,%d\n",head_hs->hs->nu,head_hs->hs->prnu,head_hs->hs->pnu,head_hs->hs->status);
		head_hs = head_hs->link;
	}
	head_hs=tmp2;
	//구매내역 리스트 쭉 읽어서 정보 보여줌.
	printf("===================================\n");
	printf("ENTER키를 누르십시오.\n");
	getchar();

	return 0;

}//상품 정보 조회 및 구매내역 조회

int prod_data_edit(Node* head_pr){

	FILE* fp;
	char* name1;
	char* name2;
	char* cate;
	int d,price;
	char c;
	Node* tmp;
	Node* tmp2;

	name1=(char*)malloc(sizeof(char)*100);
	name2=(char*)malloc(sizeof(char)*100);
	cate=(char*)malloc(sizeof(char)*100);

	tmp=(Node*)malloc(sizeof(Node));
	tmp->pr=(prod*)malloc(sizeof(prod));
	tmp2=(Node*)malloc(sizeof(Node));
	tmp2->pr=(prod*)malloc(sizeof(prod));

	tmp=head_pr;

	system("clear");
	printf("상품 정보 수정 페이지\n");
	printf("수정할 상품명 입력 : ");
	scanf("%[^\n]",name1);
	while(tmp->link!=NULL){
		if(strcmp(tmp->pr->name,name1)==0){
			break;
		}
		tmp=tmp->link;
	}
	if(tmp->link==NULL){
		getchar();
		printf("상품이 존재하지 않습니다.\n");
		getchar();
	}
	else{
		printf("1. 상품명 수정\n");
		printf("2. 카테고리 수정\n");
		printf("3. 가격 수정\n");
		printf("선택 : ");
		scanf("%d",&d);

		printf("==========================\n");
		if(d==1){
			while(1){
			printf("상품명 : ");
			scanf(" %[^\n]",name2);
			tmp2=head_pr;
			while(tmp2->link!=NULL){
				if(strcmp(tmp2->pr->name,name2)==0){
					printf("중복된 상품명이 있습니다.\n");
					getchar();
					break;
				}
				else tmp2=tmp2->link;
			}//상품명은 다른 상품과 겹칠 수 없음.
			if(tmp2->link==NULL)
				break;
			else{
				while(tmp2->link!=NULL){
					tmp2=tmp2->link;
				}
			}
			}
			strcpy(tmp->pr->name,name2);
		}
		else if(d==2){
			printf("카테고리 : ");
			scanf("%s",cate);
			strcpy(tmp->pr->category,cate);
		}
		else if(d==3){
			printf("가격 : ");
			scanf("%d",&price);
			tmp->pr->price=price;
		}

		getchar();

		fp=fopen("product.csv","w");
		fprintf(fp,"상품번호,상품명,카테고리,가격,상태\n");
		tmp=head_pr;

		while(tmp->link!=NULL){
			fprintf(fp,"%d,%s,%s,%d,%d\n",tmp->pr->nu,tmp->pr->name,tmp->pr->category,tmp->pr->price,tmp->pr->status);
			tmp=tmp->link;

		}

		fclose(fp);
	//상품정보를 수정하였으므로 product.csv를 다시 작성해줌.	
		printf("=======================\n");
		printf("변경이 완료되었습니다.\n");
		printf("ENTER키를 누르십시오.\n");
		getchar();

	}
	
	return 0;
}//상품 정보 수정

int prod_regist_del(Node* head_pr){

	FILE* fp;
	int line_nu=0;
	int d;
	char c;
	char* namecheck;
	char* name1;
	Node* newpro;
	Node* tmp;

	tmp=(Node*)malloc(sizeof(Node));
	tmp->pr=(prod*)malloc(sizeof(prod));
	newpro=(Node*)malloc(sizeof(Node));
	newpro->pr=(prod*)malloc(sizeof(prod));
	namecheck=(char*)malloc(sizeof(char)*100);
	name1=(char*)malloc(sizeof(char)*100);

	newpro=head_pr;
	tmp=newpro;

	while(newpro->link!=NULL){
		newpro=newpro->link;
	}

	printf("\n");
	newpro->pr->status=1;//상품을 새로 등록하는 경우

	fp=fopen("product.csv","a+");

	while((c=fgetc(fp))!=EOF){
		if(c=='\n'){
			line_nu++;
		}
	}//상품 번호를 입력해주기 위함

	system("clear");
	printf("상품 등록 및 삭제 페이지\n");
	printf("1. 상품 등록\n");
	printf("2. 상품 삭제\n");
	printf("선택 : ");
	scanf("%d",&d);
	getchar();

	if(d==1){

	system("clear");
	printf("상품 등록 페이지\n");//상품 등록 페이지
	while(1){
		printf("상품 이름: ");
		scanf(" %[^\n]",newpro->pr->name);
		namecheck=newpro->pr->name;//이름이 같은 상품이 있는 지 확인 하기 위함

		newpro=head_pr;
		
		while(newpro->link!=NULL){
			if(strcmp(newpro->pr->name,namecheck)==0){
				printf("중복된 상품이 있습니다.\n");//중복확인
				break;
			}
			else newpro=newpro->link;
		}

		if(newpro->link==NULL)
			break;
		else{
			while(newpro->link!=NULL)
				newpro=newpro->link;
		}

	}
	printf("카테고리: ");
	scanf("%s",newpro->pr->category);
	printf("가격: ");
	scanf("%d",&(newpro->pr->price));

	newpro->pr->nu=line_nu;

	fprintf(fp,"%d,",newpro->pr->nu);
	fprintf(fp,"%s,",newpro->pr->name);
	fprintf(fp,"%s,",newpro->pr->category);
	fprintf(fp,"%d,",newpro->pr->price);
	fprintf(fp,"%d\n",newpro->pr->status);
	
	create_Node(newpro);
	newpro=tmp;
	head_pr=newpro;

	fclose(fp);
	//상품 등록의 경우 product.csv에 추가로 작성
	getchar();
	
	printf("=======================\n");
	printf("상품등록이 완료되었습니다.\n");
	printf("ENTER키를 눌러주십시오.\n");
	getchar();
	}

	else if(d==2){//상품 삭제 페이지
	system("clear");
	printf("상품 삭제 페이지\n");
	printf("수정할 상품명 입력 : ");
	scanf("%[^\n]",name1);
	getchar();	
	tmp=head_pr;

	while(tmp->link!=NULL){
		if(strcmp(tmp->pr->name,name1)==0){
			break;
		}
		tmp=tmp->link;
	}
	if(tmp->link==NULL){
		printf("상품이 존재하지 않습니다.\n");
		getchar();
	}//수정할 상품명과 일치하는 상품이 없을 경우
	else{
		printf("상품을 삭제하려면 y, 재등록 하려면 n을 입력하세요. (y/n) : ");
		scanf(" %c",&c);
		if(c=='y'){
			tmp->pr->status=-1;
		}
		else if(c=='n'){
			tmp->pr->status=1;
		}
	
	//상품을 삭제하면 상품 상태가 -1, 삭제했던 상품을 재등록 할 수도 있으므로 재등록 또는 이미 등록되어있는 상품을 그대로 유지할 경우 상태는 1
	fp=fopen("product.csv","w");
	fprintf(fp,"상품번호,상품명,카테고리,가격,상태\n");
	tmp=head_pr;

	while(tmp->link!=NULL){
		fprintf(fp,"%d,%s,%s,%d,%d\n",tmp->pr->nu,tmp->pr->name,tmp->pr->category,tmp->pr->price,tmp->pr->status);
		tmp=tmp->link;
	}

	fclose(fp);
	//상품 정보가 바뀌었으므로 product.csv 새로 다시 작성	
	printf("=======================\n");
	printf("상품이 삭제되었습니다.\n");
	printf("ENTER키를 누르십시오.\n");
	getchar();
	}
	}

	return 0;

}//상품 등록 및 삭제

int view_user_data(Node* head){
	
	system("clear");
	printf("회원 정보 조회 페이지\n");
	printf("회원번호,이름,아이디,주소,잔액,상태\n");

	while(head->link->link!=NULL){
		printf("%d,%s,%s,%s,%d,%d\n",head->p->nu,head->p->name,head->p->id,head->p->address,head->p->money,head->p->status);
		head=head->link;
	}
	//회원 리스트 쭉 읽어서 정보 보여줌.
	printf("=======================\n");
	printf("ENTER키를 눌러주십시오.\n");

	getchar();

	return 0;
}//회원정보 조회
