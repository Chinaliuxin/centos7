#include"HttpServer.h"

using std::string;

enum boundary_type
{
  BOUNDARY_NO=0,
  BOUNDARY_FIRST,//          ------boundry
  BOUNDARY_MIDDLE,//         \r\n-----boundry\r\n
  BOUNDARY_LAST,//           \r\n -----boundry--
  BOUNDARY_BAK     //BOUNDARY的一部分
};//比如
/*
hello 
-----WebKitFormBoundary879786dfdfj
content-dispositon，，，，

这属于我们想要的boundary_middle

hello
-----WebKitFormBoundary879786dfdfj--
content-dispositon，，，，
属于boundary_last
*/

//CGI外部程序中的文件上传
class Upload
{
  private:
    int file_fd;
    string file_name;
    string first_boundary;
    string middle_boundary;
    string last_boundary;
    
    //匹配boundary字符串
    int CatchBoundary(char* buf,size_t _len,int* boundary_pos)
    {
      if(!memcmp(buf,first_boundary.c_str(),first_boundary.length()))
      {
        *boundary_pos=0;
        return BOUNDARY_FIRST;
      }
      for(size_t i=0;i<_len;i++)
      {
        if((_len-i)>=last_boundary.length())
        {

          if(!memcmp(buf+i,middle_boundary.c_str(),middle_boundary.length()))
          {
            *boundary_pos=i;
            return BOUNDARY_MIDDLE;
          }
          else if(!memcmp(buf+i,last_boundary.c_str(),last_boundary.length()))
          {
            *boundary_pos=i;
            return BOUNDARY_LAST;
          }
        }

        else 
        {
          int cmp_len=_len-i;
          if(!memcmp(buf+i,middle_boundary.c_str(),cmp_len))
          {
            *boundary_pos=i;
            return BOUNDARY_MIDDLE;
          }
          else if(!memcmp(buf+i,last_boundary.c_str(),cmp_len))
          {
            *boundary_pos=i;
            return BOUNDARY_LAST;
          }

        }
      }

      return BOUNDARY_NO;
    }


    bool GetFileName(char* buf,int* content_pos)
    {
      char* p=NULL;
      p=strstr(buf,"\r\n\r\n");
      if(p==NULL)
      {
        *content_pos=0;
        return false;
      }

      *content_pos=p-buf+4;
      string head;
      head.assign(buf,p-buf);
      string file_seq="filename=\"";
      size_t pos=head.find(file_seq);
      if(pos==string::npos)
        return false;
      string file;
      file=head.substr(pos+file_seq.length());
      pos=file.find("\"");
      file.erase(pos);
      file_name="WWW/"+file;

      return true;
    }


    bool WriteFile(char* buf,int len)
    {
      if(file_fd!=-1)
      { 
        write(file_fd,buf,len);
        return true;
      }
      return false;
    }

    bool CreateFile()
    {
      file_fd=open(file_name.c_str(),O_CREAT|O_WRONLY,0644);
	  /*
	  O_RDONLY 只读打开
O_WRONLY 只写打开
O_RDWR  可读可写打开
以下可选项可以同时指定0个或多个,和必选项按位或起来作为flags 参数。可选项有很多,这
里只介绍一部分,其它选项可参考open(2)的Man Page:
O_APPEND   表示追加。如果文件已有内容,这次打开文件所写的数据附加到文件的末尾而不
					 覆盖原来的内容。
O_CREAT 若此文件不存在则创建它。使用此选项时需要提供第三个参数mode ,表示该文件
				 的访问权限。
O_EXCL 如果同时指定了O_CREAT,并且文件已存在,则出错返回。
O_TRUNC  如果文件已存在,并且以只写或可读可写方式打开,则将其长度截断
				   (Truncate)为0字节。

	  */
      if(file_fd<0)
      {
        perror("open file");
        return false;
      }
      return true;
    }

    bool CloseFile()
    {
      if(file_fd!=-1)
      {
        close(file_fd);
        file_fd=-1;
      }
      return true;
    }




  public:
    Upload():file_fd(-1){}

    ~Upload()
    {
      close(file_fd);
    }

    bool InitUpload()
    {
      //获取boundary
      umask(0);
      char* p=getenv("Content-Type");
      if(p==NULL)
      {
        std::cerr<<"Content-Type error";
        return false;
      }
      string find_boundary="boundary=";
      string content_type=p;
      size_t pos=content_type.find(find_boundary);
      if(pos==string::npos)
      {
        std::cerr<<"not find boundary";
        return false;
      }
      string boundary=content_type.substr(pos+find_boundary.length());
      first_boundary="--"+boundary;
      middle_boundary="\r\n"+first_boundary+"\r\n";
      last_boundary="\r\n"+first_boundary+"--\r\n";
      return true;
    }

    bool ProcessUpload()
    {
      //len表示已读的长度，_len表示还有的长度。
      int64_t len=0,_len=0;
      string content_len=getenv("Content-Length");
      int64_t file_len=atoi(content_len.c_str());
      char buf[MAX_BUF]={0};
      
      while(len < file_len)
      {
        int rlen=read(0,buf+_len,MAX_BUF);
        len+=rlen;
        _len+=rlen;
        //boundary起始位置
        int boundary_pos=0;
        //文件起始位置
        int content_pos=0;
      

        int flag=this->CatchBoundary(buf,_len,&boundary_pos);
        if(flag == BOUNDARY_FIRST)
        {
          if(GetFileName(buf,&content_pos))
          {
            CreateFile();
            _len-=content_pos;
            //用后面的有效数据向前移至起始位置
            memmove(buf,buf+content_pos,_len);
          }
          else
          {
            _len-=first_boundary.length();
            memmove(buf,buf+first_boundary.length(),_len);
          }
        }


        while(1)
        {
          int flag=CatchBoundary(buf,_len,&boundary_pos);
          if(flag!=BOUNDARY_MIDDLE)
          {
            break;
          }
          WriteFile(buf,boundary_pos);
          CloseFile();
          _len-=boundary_pos;
          memmove(buf,buf+boundary_pos,_len);
          if(GetFileName(buf,&content_pos))
          {
            CreateFile();
            _len-=content_pos;
            memmove(buf,buf+content_pos,_len);
          }
          else
          {
            if(content_pos==0)
              break;
            _len-=middle_boundary.length();
            memmove(buf,buf+middle_boundary.length(),_len);
          }
        }


        int tmp1=CatchBoundary(buf,_len,&boundary_pos);
        if(tmp1==BOUNDARY_LAST)
        {
          WriteFile(buf,boundary_pos);
          CloseFile();
          return true;
        }
        int tmp2=CatchBoundary(buf,_len,&boundary_pos);
        if(tmp2==BOUNDARY_BAK)
        {
          WriteFile(buf,boundary_pos);
          _len-=boundary_pos;
          memmove(buf,buf+boundary_pos,_len);
        }
        int tmp3=CatchBoundary(buf,_len,&boundary_pos);
        if(tmp3==BOUNDARY_NO)
        {
          WriteFile(buf,_len);
          _len=0;
        }
      }

      return true;
    }
};





int main()
{

  std::cerr<<"hallow"<<std::endl;
  Upload up;
  string rsp_body;
  if(up.InitUpload()==false)
  {

    std::cerr<<"Init"<<std::endl;
    rsp_body="<html><body><h1>500<h1></body></html>\r\n";
    write(1,rsp_body.c_str(),rsp_body.length());
    return 1;
  }
  
  std::cerr<<"uploading !"<<std::endl;

  if(up.ProcessUpload()==false)
  {
    std::cerr<<"upload"<<std::endl;
    rsp_body="<html><body><h1>404<h1></body></html>\r\n";
    write(1,rsp_body.c_str(),rsp_body.length());
    return 1;
  }
  else
    rsp_body="<html><body><h1>SUCCESS<h1></body></html>\r\n";
  write(1,rsp_body.c_str(),rsp_body.length());
  return 0;
}




