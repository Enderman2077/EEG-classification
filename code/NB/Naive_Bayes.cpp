#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define pi 3.14159265
#define M 424//ѵ��������
#define N 106//����������
#define C 13

struct SAM {
	double feature[160];//160ά�Ե粨��������
	int id;//�����߱��
	int label;//���öȱ�ǩ��ֻ��ȡ11��12��21��22 
	int emo;//��б�ǩ
};

int main() {
	FILE *fpr,*fpw;//��д�ļ�ָ��
	int i,j,k,cnt[C],right,res;
	int count1[C][4],count2[C][13];
	double pc[C],mean[C][160],var[C][160],sum[C][160],pt[C],max;
	double pxc1[C][4];//���û��Ѷȱ�ǩ�������ܶ� 
	double pxc2[C][13];//��б�ǩ�������ܶ� 
	double correct;
	int h[N];//���Խ��
	struct SAM sample[M];
	struct SAM test[N];
	fpr=fopen("MAHNOB-HCI_data1.txt","r");//��ȡ����1
	for(i=0;i<N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data2.txt","r");//��ȡ����2
	for(i=N;i<2*N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data3.txt","r");//��ȡ����3
	for(i=2*N;i<3*N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	fpr=fopen("MAHNOB-HCI_data4.txt","r");//��ȡ����4
	for(i=3*N;i<M;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&sample[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &sample[i].id, &sample[i].label,&sample[i].emo);
	}
	//����������� 
	for(i=0;i<C;i++) cnt[i]=0;
	for(i=0;i<M;i++){
		cnt[sample[i].emo]++;
	}
	for(i=0;i<C;i++){//������˹���� 
		pc[i]=(double)(cnt[i]+1)/(M+C);
	}
	//������������ 
	for(i=0;i<C;i++){//��ʼ�� 
		for(j=0;j<160;j++) sum[i][j]=0.0;
	}
	for(i=0;i<M;i++){//����ѵ���� 
		for(j=0;j<160;j++){
			sum[sample[i].emo][j]+=sample[i].feature[j];
		}
	}
	for(i=0;i<C;i++){//ƽ��ֵ 
		for(j=0;j<160;j++){
			mean[i][j]=(double)sum[i][j]/cnt[i];
		}
	}
	for(i=0;i<C;i++){//��ʼ�� 
		for(j=0;j<160;j++) sum[i][j]=0.0;
	} 
	for(i=0;i<M;i++){//����ѵ���� 
		for(j=0;j<160;j++){
			sum[sample[i].emo][j]+=pow(sample[i].feature[j]-mean[sample[i].emo][j],2);
		}
	}
	for(i=0;i<C;i++){//����
		for(j=0;j<160;j++){
			var[i][j]=(double)sum[i][j]/cnt[i];
		}
	}
	//�����ǩ����������
	for(i=0;i<C;i++){//��ʼ�� 
		for(j=0;j<4;j++) count1[i][j]=0;
	}
	for(i=0;i<M;i++){
		switch(sample[i].label){//ͳ��label��ǩ 
			case 11:{
				count1[sample[i].emo][0]++;
				break;
			}
			case 12:{
				count1[sample[i].emo][1]++;
				break;
			}
			case 21:{
				count1[sample[i].emo][2]++;
				break;
			}
			case 22:{
				count1[sample[i].emo][3]++;
				break;
			}
			default:printf("error!\n");
		}
	}
	for(i=0;i<C;i++){
		for(j=0;j<4;j++){//������˹����
			pxc1[i][j]=(double)(count1[i][j]+1)/(M+4);
		} 
	}
	/*for(i=0;i<C;i++){//������б�ǩ���������� 
		for(j=0;j<13;j++) count2[i][j]=0;
	}
	for(i=0;i<M;i++){
		count2[sample[i].id-1][sample[i].emo]++; 
	}
	for(i=0;i<C;i++){
		for(j=0;j<13;j++){//������˹����
			pxc2[i][j]=(double)(count2[i][j]+1)/(M+13);
		}
	}
	*/
	//��֤�׶� 
	fpr=fopen("MAHNOB-HCI_data5.txt","r");//��ȡ����5
	for(i=0;i<N;i++){
		for(j=0;j<160;j++){
			fscanf(fpr,"%lf",&test[i].feature[j]);
		}
		fscanf(fpr,"%d%d%d", &test[i].id, &test[i].label,&test[i].emo);
	}
	for(i=0;i<N;i++){//������ʹ���
		for(j=0;j<C;j++){
			pt[j]=pc[j];//������ɢ�������� 
			for(k=0;k<160;k++){//���������������� 
				pt[j]*=exp(-pow(test[i].feature[k]-mean[j][k],2)/(2*var[j][k]))/sqrt(2*pi*var[j][k]);
			}
			switch(test[i].label){//���ݲ���������labelѡ����Ӧ���������� 
				case 11:{
					pt[j]*=pxc1[j][0];
					break;
				}
				case 12:{
					pt[j]*=pxc1[j][1];
					break;
				}
				case 21:{
					pt[j]*=pxc1[j][2];
					break;
				}
				case 22:{
					pt[j]*=pxc1[j][3];
					break;
				}
			}
			//pt[j]*=pxc2[j][test[i].emo];//������б�ǩ�������� 
		}
		max=-1;
		for(j=0;j<C;j++){
			if(pt[j]>max){
				max=pt[j];
				res=j;
			}
		}
		h[i]=res;//�ó������� 
	}
	printf("������������Ϊ��\n");//��������� 
	for(i=0;i<N;i++) printf("%d ",h[i]);
	printf("\n");
	
	//������ȷ�� 
	right=0;
	for(i=0;i<N;i++){
		if(h[i]==test[i].emo) right++;//ͳ����ȷ������ 
	}
	correct=(double)right/N;
	printf("������ȷ����%d\n�����������%d\n��ȷ��Ϊ��%lf",right,N-right,correct); 
	fclose(fpr);//�ر��ļ�ָ��
	return 0;
}
