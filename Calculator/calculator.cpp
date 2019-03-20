#include<iostream>
#include<stack>
#include<cmath>

using namespace std;

stack<double> num;//���ֶ�ջ
stack<char> op;//�������ջ

int nagNuFlag = 1;
/*
������ǣ�0����֮������ġ�-������������ǣ�
1����֮������ġ�-����������ǣ�
*/
int squareFlag = 0;
/*
�˷�������0�������г˷���
1�����ȡ���˷���ǣ��ȴ��ݵĶ��룬
2�����ݵĶ�����ɣ��������г˷�����*/

double getInfo(string exp);//���ַ����л�ȡ���ּ��������Ϣ
int getNum(string exp);//���ַ����л�ȡ������Ϣ
int getOp(char exp, int opType);//���ַ����л�ȡ�������Ϣ
void getRes(char op);//���� num1 op num2 ��ֵ��������numջ��
int getOpType(char op);//��ȡ��������ȼ���2��ߣ�0���

double getInfo(string exp) {
	int length = 0;
	int opType = 0;
	
	int tmp;
	while ((tmp = exp.find("qrt")) > -1) {
		exp.erase(tmp, 3);
	}
	length = exp.size();

	for (int i = 0; i < length;) {//i������ǵ�ǰ��ȡλ��
		int j = (exp[i] <= '9' && exp[i] >= '0') ? 1 : 0;//�ж϶������Ƿ�Ϊ���֣�trueΪ1��falseΪ0
		if (j) {			
			i += getNum(&exp[i]);//��ȡ�������֣�������ȡ����ƶ�������֮��
			if (nagNuFlag == 1)nagNuFlag = 0;//����������		
		}
		else {
			//�����˷����
			if (exp[i] == '^') {
				i++;
				squareFlag = 1;
				continue;
			}

			opType = getOp(exp[i], opType);//��ȡ��ǰ�������ջ�е�������ȼ���������Ҫʱ�������������Ӧ�Ĳ���
			i++;//��ȡλ���ƶ��������֮��
		}
		//���ֱ���������󣬶�����г˷�����
		if ((squareFlag == 1) && (exp[i] != '.')) {
			int Num = num.top();
			num.pop();
			num.top() = pow(num.top(), Num);
			squareFlag = 0;
		}
		
	}
	//����ʽ��ȡ��ɺ����������ջ�ǿգ������ڵ���������ջ�������������������Ӧ�Ĳ���
	for (; op.size() > 0;) {
		getRes(op.top());
		op.pop();
	}

	return num.top();
}
//��ȡ�������֣������ض�ȡ��������
int getNum(string exp) {
	int length = 0;
	int isFirst = 1;
	int i = 0;

	length = exp.size();

	for (; i < length; i++) {
		char tmp = exp[i];
		int isNum = (tmp <= '9' && tmp >= '0') ? 1 : 0;
		if (isNum && isFirst) {
			num.push(tmp - 48);
			isFirst = 0;
		}
		else if (isNum) {
			num.top() *= 10;
			num.top() += tmp - 48;
		}
		else break;
	}
	if (exp[i] == '.') {
		i++;//����С����
		i += getNum(&exp[i]);//��ȡС����֮����������֣�������ȡ����ƶ�������֮��
		for (; num.top() >= 1;) {//ͨ������10������С����������ת��ΪС��
			num.top() /= 10;
		}
		double Num = num.top();
		num.pop();
		num.top() += Num;//���洢��С����ջ���ӵ�С����ǰ��������
	}
	return i;
}
//��ȡ����ʽ�е������������������ȼ�������Ӧ�Ĳ���
int getOp(char exp, int opType) {
	switch (exp) {
		//��������������ջ��������ǰ��ջ������ȼ���Ϊ0
	case '(': {
		if (squareFlag > 0)squareFlag++;//�Գ˷����ŵ����е����Ų������м���

		op.push(exp);
		opType = 0;
		nagNuFlag = 1;
		return opType;
		break;
	}
	case '+': {
		//����+���ҵ�ǰ������ȼ�С�ڡ�+�������ȼ���������ջ
		if (opType < 1) {
			op.push(exp);
			opType = 1;
			return opType;
		}
		//��ǰ������ȼ����ڡ����ڡ�+�������ȼ���������ջ��������
		else {
			for (; opType >= 1;) {
				getRes(op.top());//�Գ�ջ�������������Ӧ���������
				op.pop();
				//��ȡ��ǰ�������ջ�е�������ȼ�
				if (op.size() > 0) {
					opType = getOpType(op.top());
				}
				else {
					opType = 0;
				}
			}
			//ֱ����ǰ������ȼ�С�ڡ�+�������ȼ���������ջ
			op.push(exp);
			opType = 1;
			return opType;
		}
		break;
	}
//����ͬ��
	case '-': {
		if (opType < 1) {
			if (nagNuFlag == 1) {
				nagNuFlag = 0;
				op.push('N');
				opType = 3;
				return opType;
			}
			else {
				op.push(exp);
				opType = 1;
				return opType;
			}
		}
		else {
			for (; opType >= 1;) {
				getRes(op.top());
				op.pop();
				if (op.size() > 0) {
					opType = getOpType(op.top());
				}
				else {
					opType = 0;
				}

			}
			op.push(exp);
			opType = 1;
			return opType;
		}
		break;
	}
	case '*': {
		if (opType < 2) {
			op.push(exp);
			opType = 2;
			return opType;
		}
		else {
			for (; opType >= 2;) {
				getRes(op.top());
				op.pop();
				if (op.size() > 0) {
					opType = getOpType(op.top());
				}
				else {
					opType = 0;
				}
			}
			op.push(exp);
			opType = 2;
			return opType;
		}
		break;
	}
	case '/': {
		if (opType < 2) {
			op.push(exp);
			opType = 2;
			return opType;
		}
		else {
			for (; opType >= 2;) {
				getRes(op.top());
				op.pop();
				if (op.size() > 0) {
					opType = getOpType(op.top());
				}
				else {
					opType = 0;
				}
			}
			op.push(exp);
			opType = 2;
			return opType;
		}
		break;
	}  
	case ')': {//����������ʱ�����������ջ���г�ջ����ֱ������������
		if (squareFlag > 1)squareFlag--;

		for (; op.top() != '(';) {
			getRes(op.top());
			op.pop();
		}
		op.pop();
		if (op.size() > 0) {
			opType = getOpType(op.top());
		}
		else {
			opType = 0;
		}
		return opType;
		break;
	}
	case 'S': {
		op.push(exp);
		opType = 4;
		return opType;
	}
	}
	return opType;
}
//��ȡ��Ӧ�������������
void getRes(char op) {
	switch (op)
	{
	case '+': {
		double Num = num.top();
		num.pop();
		num.top() += Num;
		break;
	}
	case '-': {
		double Num = num.top();
		num.pop();
		num.top() -= Num;
		break;
	}
	case '*': {
		double Num = num.top();
		num.pop();
		num.top() *= Num;
		break;
	}
	case '/': {
		double Num = num.top();
		num.pop();
		num.top() /= Num;
		break;
	}
	case 'N': {
		num.top() = 0 - num.top();
		break;
	}
	case 'S': {
		num.top() = sqrt(num.top());
		break;
	}
	default:
		break;
	}
}
//��ȡ��ǰ�����ջ�е�������ȼ�
int getOpType(char op) {
	switch (op) {
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case 'N':return 3;
	case 'S':return 4;
	default: return 0;
	}
}