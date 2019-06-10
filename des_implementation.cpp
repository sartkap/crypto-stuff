#include<bits/stdc++.h>
using namespace std;


// table to convert 64 bit key into 56 bit key generator.
int key_parity_drop[56]=  
    {   57,49,41,33,25,17,9, 
        1,58,50,42,34,26,18, 
        10,2,59,51,43,35,27, 
        19,11,3,60,52,44,36,           
        63,55,47,39,31,23,15, 
        7,62,54,46,38,30,22, 
        14,6,61,53,45,37,29, 
        21,13,5,28,20,12,4 
    };

// table to keep track of how many places key
// has to be shifted in each of the 16 rounds 
// of DES
int key_shift_in_each_round[16]= 
    {   1, 1, 2, 2, 
        2, 2, 2, 2,  
        1, 2, 2, 2,  
        2, 2, 2, 1 
    };

// table to convert 56 bit key generator into 
// key for each round
int compressed_key[48]= 
    {   14,17,11,24,1,5, 
        3,28,15,6,21,10, 
        23,19,12,4,26,8, 
        16,7,27,20,13,2, 
        41,52,31,37,47,55, 
        30,40,51,45,33,48, 
        44,49,39,56,34,53, 
        46,42,50,36,29,32 
    };

// the following two vectors store all the 16 keys
// used in binary and hexadecimal format respectively
vector<string> binary_keys;
vector<string> hexa_keys;

// function to xor two binary strings
string xor_func(string s1, string s2){
    string result="";
    for(int i=0;i<s1.size();i++){
        if(s1[i]==s2[i])result+="0";
        else result+="1";
    }
    return result;

}

// convert hexadecimal string to binary
string convertToBinary(string s){
    string result = "";
    for(int i=0;i<s.size();i++){
        if(s[i]=='0')result += "0000";
        else if(s[i]=='1')result += "0001";
        else if(s[i]=='2')result += "0010";
        else if(s[i]=='3')result += "0011";
        else if(s[i]=='4')result += "0100";
        else if(s[i]=='5')result += "0101";
        else if(s[i]=='6')result += "0110";
        else if(s[i]=='7')result += "0111";
        else if(s[i]=='8')result += "1000";
        else if(s[i]=='9')result += "1001";
        else if(s[i]=='A')result += "1010";
        else if(s[i]=='B')result += "1011";
        else if(s[i]=='C')result += "1100";
        else if(s[i]=='D')result += "1101";
        else if(s[i]=='E')result += "1110";
        else if(s[i]=='F')result += "1111";

    }
    return result;
}

// convert binary string to hexadecimal
string convertToHexadecimal(string s){
    string result = "";
    for(int i=0;i<s.size();i+=4){
        string temp = "";
        temp+=s[i];
        temp+=s[i+1];
        temp+=s[i+2];
        temp+=s[i+3];
        if(temp == "0000")result+="0";
        else if(temp == "0001")result+="1";
        else if(temp == "0010")result+="2";
        else if(temp == "0011")result+="3";
        else if(temp == "0100")result+="4";
        else if(temp == "0101")result+="5";
        else if(temp == "0110")result+="6";
        else if(temp == "0111")result+="7";
        else if(temp == "1000")result+="8";
        else if(temp == "1001")result+="9";
        else if(temp == "1010")result+="A";
        else if(temp == "1011")result+="B";
        else if(temp == "1100")result+="C";
        else if(temp == "1101")result+="D";
        else if(temp == "1110")result+="E";
        else if(temp == "1111")result+="F";
    }
    return result;
}

// change one string into another based on the parity_table argument passed
string permuteString(string key,int* parity_table, int n){
    string result = "";
    for(int i=0;i<n;i++){
        result += key[parity_table[i]-1];
    }
    return result;
}

// shift string num_shifts unit to left
string shift_left(string s, int num_shifts){
    
    for(int i=0;i<num_shifts;i++){
        string result = "";
        for(int j=1;j<s.size();j++){
            result += s[j];
        }
        result += s[0];
        s = result;
    }
    return s;
}

/*  function used to both encrypt and decrypt
    text since encryption and decryption follow same
    steps (only keys are used opposite order) 
 */ 

/*  The steps are - 
    1. initial permutation is applied 
    2. plain text is divided into two equal parts - 'left' and 'right'
    Now repeat the following 16 times -  
        3. expand right part from 32 bits to 48 bits.
        4. now xor this with the ith key.
        5. apply s-box
        6. apply p-box
        7. xor result with left and put left equal to this
        8. swap left and right
    9. recombine left and right and apply inverse permutation 
*/
string encrypt(string plain_text){
    plain_text = convertToBinary(plain_text);

    int initial_permutation[64]=  
    {   58,50,42,34,26,18,10,2, 
        60,52,44,36,28,20,12,4, 
        62,54,46,38,30,22,14,6, 
        64,56,48,40,32,24,16,8, 
        57,49,41,33,25,17,9,1, 
        59,51,43,35,27,19,11,3, 
        61,53,45,37,29,21,13,5, 
        63,55,47,39,31,23,15,7 
    }; 

    plain_text = permuteString(plain_text, initial_permutation, 64);

    string left, right;
    for(int i=0;i<32;i++){
        left.push_back(plain_text[i]);
    }
    for(int i=32;i<64;i++){
        right.push_back(plain_text[i]);
    }
    // table to expand right from 32 to 48 bits.
    int right_expansion_permutation[48]=  
    {   32,1,2,3,4,5,4,5, 
        6,7,8,9,8,9,10,11, 
        12,13,12,13,14,15,16,17, 
        16,17,18,19,20,21,20,21, 
        22,23,24,25,24,25,26,27, 
        28,29,28,29,30,31,32,1 
    }; 


    int s_box[8][4][16]= 
    {{ 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }, 
    { 
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 
    }, 
  
  
    { 
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 
    }, 
    { 
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 
    }, 
    { 
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 
    }, 
    { 
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 
    }, 
    { 
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 
    }, 
    { 
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
    }}; 

    int p_box[32]= 
    {   16,7,20,21, 
        29,12,28,17, 
        1,15,23,26, 
        5,18,31,10, 
        2,8,24,14, 
        32,27,3,9, 
        19,13,30,6, 
        22,11,4,25 
    }; 

    // 16 cycle DES applied
    for(int i=0;i<16;i++){
        string expanded_right = permuteString(right, right_expansion_permutation, 48);

        string x = xor_func(expanded_right,binary_keys[i]);
        string temp = "";

        // S-box applied
        for(int i=0;i<8;i++){
            int num1 = 2*int(x[i*6]-'0')+int(x[i*6+5]-'0');
            int num2 = 8*int(x[i*6 + 1]-'0') + 4*int(x[i*6+2]-'0') + 2*int(x[i*6+3]-'0')+int(x[i*6+4]-'0');
            int num_result = s_box[i][num1][num2];
            temp+=char(num_result/8 + '0');
            num_result%=8;
            temp+=char(num_result/4 + '0');
            num_result%=4;
            temp+=char(num_result/2 + '0');
            num_result%=2;
            temp+=char(num_result + '0');
        }

        temp = permuteString(temp,p_box,32);
        x = xor_func(temp, left);
        left = x;

        if(i!=15){
            swap(left,right);
        }


    }
    string combined = left + right;
    int initial_permutation_inverse[64]=  
    {   40,8,48,16,56,24,64,32, 
        39,7,47,15,55,23,63,31, 
        38,6,46,14,54,22,62,30, 
        37,5,45,13,53,21,61,29, 
        36,4,44,12,52,20,60,28, 
        35,3,43,11,51,19,59,27, 
        34,2,42,10,50,18,58,26, 
        33,1,41,9,49,17,57,25 
    }; 
    string cipher = convertToHexadecimal(permuteString(combined, initial_permutation_inverse, 64));

    return cipher;
}

int main(){
    string key = "AABBCCDDEEFF0011";
    string plain_text = "1234567890ABCDEF";
    //cout<<sizeof(plain_text);
    key = convertToBinary(key);

    // make key generator
    key = permuteString(key,key_parity_drop,56); 
    
    // divide into two parts of 28 bits each
    string left;
    string right;
    for(int i=0;i<28;i++){
        left.push_back(key[i]);
    }
    for(int i=28;i<56;i++){
        right.push_back(key[i]);
    }

    



    // all 16 keys generated and stored
    for(int i=0;i<16;i++){
        left = shift_left(left,key_shift_in_each_round[i]);
        right =  shift_left(right,key_shift_in_each_round[i]);

        string combined = left + right;

        string final_key = permuteString(combined, compressed_key, 48);

        binary_keys.push_back(final_key);
        hexa_keys.push_back(convertToHexadecimal(final_key));

    }
    // encrypt plain text
    string cipher_text = encrypt(plain_text);
    cout<<"Cipher::\n"<<cipher_text<<"\n";
    // reverse order of keys for decryption
    reverse(binary_keys.begin(), binary_keys.end());
    reverse(hexa_keys.begin(), hexa_keys.end());

    // decrypt data
    string original_text = encrypt(cipher_text);
    cout<<"Plain Text::\n"<<original_text<<"\n";
    return 0;
}