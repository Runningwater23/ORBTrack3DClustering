 /*
 ����: XuBin&&Jason
 ����: 190662513@qq.com
 ά��: 2014.3.8
 */
#include <vector>

using namespace std;

const int DIME_NUM = 4;        //����ά�ȣ�ȫ�ֳ���

//���ݵ�����
class DataPoint
{
private:
	unsigned long dpID;                //���ݵ�ID
	double dimension[DIME_NUM];        //ά������
	long clusterId;                    //��������ID
	bool isKey;                        //�Ƿ���Ķ���
	bool visited;                    //�Ƿ��ѷ���
	vector<unsigned long> arrivalPoints;    //�������ݵ�id�б�
public:
	DataPoint(){};                                                    //Ĭ�Ϲ��캯��
	DataPoint(unsigned long dpID,double* dimension , bool isKey);    //���캯��
	
	unsigned long GetDpId();                //GetDpId����
	void SetDpId(unsigned long dpID);        //SetDpId����
	double* GetDimension();                    //GetDimension����
	void SetDimension(double* dimension);    //SetDimension����
	bool IsKey();                            //GetIsKey����
	void SetKey(bool isKey);                //SetKey����
	bool isVisited();                        //GetIsVisited����
	void SetVisited(bool visited);            //SetIsVisited����
	long GetClusterId();                    //GetClusterId����
	void SetClusterId(long classId);        //SetClusterId����
	vector<unsigned long>& GetArrivalPoints();    //GetArrivalPoints����
};

////Ĭ�Ϲ��캯��
//DataPoint::DataPoint()
//{
//}

//���캯��
inline DataPoint::DataPoint(unsigned long dpID, double* dimension, bool isKey) :isKey(isKey), dpID(dpID)
{
	//����ÿά��ά������
	for(int i=0; i<DIME_NUM;i++)
	{
		this->dimension[i]=dimension[i];
	}
}

//����ά������
inline void DataPoint::SetDimension(double* dimension)
{
	for(int i=0; i<DIME_NUM;i++)
	{
		this->dimension[i]=dimension[i];
	}
}

//��ȡά������
inline double* DataPoint::GetDimension()
{
	return this->dimension;
}

//��ȡ�Ƿ�Ϊ���Ķ���
inline bool DataPoint::IsKey()
{
	return this->isKey;
}

//���ú��Ķ����־
inline void DataPoint::SetKey(bool isKey)
{
	this->isKey = isKey;
}

//��ȡDpId����
inline unsigned long DataPoint::GetDpId()
{
	return this->dpID;
}

//����DpId����
inline void DataPoint::SetDpId(unsigned long dpID)
{
	this->dpID = dpID;
}

//GetIsVisited����
inline bool DataPoint::isVisited()
{
	return this->visited;
}


//SetIsVisited����
inline void DataPoint::SetVisited(bool visited)
{
	this->visited = visited;
}

//GetClusterId����
inline long DataPoint::GetClusterId()
{
	return this->clusterId;
}

//GetClusterId����
inline void DataPoint::SetClusterId(long clusterId)
{
	this->clusterId = clusterId;
}

//GetArrivalPoints����
inline vector<unsigned long>& DataPoint::GetArrivalPoints()
{
	return arrivalPoints;
}

