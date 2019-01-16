
/* Đề Thi - Lập Trình Song Song
 Thời Gian: 60 phút
 Yêu cầu: Cho trước chương trình C dưới đây. Yêu cầu sinh viên viết chương trình song song sử dụng OpenMP giải bài toán này.
 */

#include <stdio.h>
//#include <malloc.h>
#include <math.h>
#include <omp.h>

#define  C       1
#define  m       20
#define  T       5
#define  dt      0.01
//=========================
void DisplayMatrix(float *A, int row,  int col)
{
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++) printf("  %f",*(A+i*col+j));
        printf("\n");
    }
}
//=========================
void KhoiTao(float *Phi_old,float *Phi_current)
{
    int i;
    float dx,X;
    dx = 1.0/(m-1);
    for (i = 0 ; i < m ; i++){
        X = i*dx;
        *(Phi_old+i)= sin(2.0*M_PI*X);
        *(Phi_current+i) = *(Phi_old+i);
    }
}
//=========================
void FD(float *Phi_current,float *dPhi)
{
    int i;
    float c,l,r;
    for (i = 0 ; i < m ; i++){
        c = *(Phi_current+i);
        l = (i==0)    ? - *(Phi_current+i+1) : *(Phi_current+i-1);
        //left neighbor = -r to dPhi=0
        r = (i==m-1) ? - *(Phi_current+i-1) : *(Phi_current+i+1);
        //right neighbor = -l to dPhi=0
        *(dPhi+i) = l - 2*c + r;
    }
}
//=========================
int main()
{
    int i,nTime,id,stt,stp;
    float dx,t,tau;
    t = 0.0;
    float *Phi_old,*Phi_current,*Phi_new,*dPhi;
    Phi_old     = (float *) malloc (m*sizeof(float));
    Phi_current = (float *) malloc (m*sizeof(float));
    Phi_new     = (float *) malloc (m*sizeof(float));
    dPhi        = (float *) malloc (m*sizeof(float));
    KhoiTao(Phi_old,Phi_current);
    dx = 1.0/(m-1);
    tau = C*dt/dx;
    nTime=T/dt;
    omp_set_num_threads(10);
//    while (t<=T)
//    {
//        FD(Phi_current, dPhi);
//        for (i = 0 ; i < m ; i++)
//            *(Phi_new+i) = 2*(*(Phi_current+i)) - *(Phi_old+i) + tau*tau*(*(dPhi+i));
//        for (i = 0 ; i < m ; i++){
//            *(Phi_old+i)      = *(Phi_current+i);
//            *(Phi_current+i)  = *(Phi_new+i);
//        }
//        t=t+dt;
//    }
    
    #pragma omp parallel private(id,i,stt,stp)
    {
        id = omp_get_thread_num();
        stt = id*nTime; stp = id*nTime+nTime;
        for (t=stt;t<stp;t++){
               FD(Phi_current, dPhi);
                for (i = 0 ; i < m ; i++)
                    *(Phi_new+i) = 2*(*(Phi_current+i)) - *(Phi_old+i) + tau*tau*(*(dPhi+i));
                for (i = 0 ; i < m ; i++){
                    *(Phi_old+i)      = *(Phi_current+i);
                    *(Phi_current+i)  = *(Phi_new+i);
                }
            }
            
        }
    printf("Gia tri cuoi cung:\n");
    DisplayMatrix(Phi_current, m, 1);
    //
    return 0;

    
}
    



