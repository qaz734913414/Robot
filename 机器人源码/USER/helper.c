#include "helper.h"

void Delay(u32 nCount)
{
  while(nCount--);
}
void Delay_ums(u32 n){
  u32 i = 168;
	for( ; i > 0;i--){
		Delay(n);
	}
}
void Delay_mms(u32 n){
  u32 i = 999;
	for( ; i > 0;i--){
		Delay_ums(n);
	}
}
void Delay_ms(u32 n){
	u32 i = 998;
	for( ; i > 0;i--){
		Delay_mms(n);
	}
}

