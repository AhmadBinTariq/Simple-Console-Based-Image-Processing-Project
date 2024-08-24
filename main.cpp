#include <iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<math.h>

    using namespace std;

    struct Image {
        char ImageFileName[100];
        vector<vector<int>> ImageData;
        int cols, rows, maxGray;
        vector<char> comment;
        bool is_swapped = false;
        bool imageLoaded;
        bool imageModified;

        void changeBrightness(double factor) {
            for (int r = 0; r < rows; r++)
                for (int c = 0; c < cols; c++) {
                    ImageData[r][c] *= factor;
                    if (ImageData[r][c] > maxGray)
                        ImageData[r][c] = maxGray;
                }
        }

        int loadImage(char ImageName[]) {

            ifstream FCIN(ImageName);

            if (!FCIN.is_open())
                return -1;

            char MagicNumber[5];
            char Comment[100];

            FCIN.getline(MagicNumber, 4);
            FCIN.getline(Comment, 100);
            FCIN >> cols >> rows >> maxGray;

            ImageData.clear();
            if (rows>cols)
            {
                ImageData.resize(rows, vector<int>(rows));
            }
            else
             ImageData.resize(cols, vector<int>(cols));

            for (int r = 0; r < rows; r++)
                for (int c = 0; c < cols; c++)
                    ImageData[r][c]=maxGray;
            
            for (int r = 0; r < rows; r++)
                for (int c = 0; c < cols; c++)
                    FCIN >> ImageData[r][c];

            if (FCIN.fail())
                return -2;

            FCIN.close();
            imageLoaded = true;
            imageModified = false;
            strcpy(ImageFileName, ImageName);
            return 0;
        }

        int saveImage(char ImageName[]) {
            ofstream FCOUT(ImageName);
            if (!FCOUT.is_open())
                return -1;

            FCOUT << "P2\n# This is a comment\n"
                << cols << " " << rows << endl << maxGray << endl;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++)
                    FCOUT << ImageData[r][c] << " ";
                FCOUT << endl;
            }
            FCOUT.close();
            system(ImageName);
            imageModified = false;
            return 0;
        }

        void horzontalFlipImage() {
            for (int r = 0; r < rows / 2; r++)
                for (int c = 0; c < cols; c++) {
                    int T = ImageData[r][c];
                    ImageData[r][c] = ImageData[rows - r-1][c];
                    ImageData[rows - r-1][c] = T;
                }
            return;
        }

        void Rot_to_right() {

           for (int r = 0; r < rows; r++) {
                for (int c = r; c < cols; c++) {
                    swap(ImageData[r][c], ImageData[c][r]);
                }
           }
            swap(cols, rows);
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols / 2; c++)
                {
                    swap(ImageData[r][c], ImageData[r][cols - c]);
                }
            }

            is_swapped = true;
        }

        void rot_to_lef() {
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols / 2; c++)
                {
                    swap(ImageData[r][c], ImageData[r][cols - c-1]);
                }
            }
            for (int r = 0; r < rows; r++)
            {
                for (int c = r + 1; c < cols; c++)
                {
                    swap(ImageData[r][c], ImageData[c][r]);
                }
            }
            swap(cols, rows);
            is_swapped = true;

        }

        void Rot_to_theeta() {
            double angle;
            cout << "Enter the angle of rotation(in degree)(negative angle for opposite rotation):";
            cin >> angle;
            angle *= 0.0175;
            vector <vector<int>> temp(rows, vector<int>(cols,0));
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
                    int x = (c * cos(angle) - r * sin(angle));
                    int y = (c * sin(angle) + r * cos(angle));
                    
                    if (x>0 && x < cols && y < rows && y>0)
                        temp[r][c] = ImageData[y][x];
                    
                }
            }
            ImageData = temp;
        }

        void cnvrt_to_bin() {
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
                    if (ImageData[r][c] > maxGray / 2)
                    {
                        ImageData[r][c] = maxGray;
                    }
                    else
                        ImageData[r][c] = 0;
                }
            }
        }

        void vertical_flip() {
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols/2; c++)
                {
                    swap(ImageData[r][c], ImageData[r][cols-c-1]);
                }
            }

        }

        void resize_by_ratio() {
            int col=0, row=0 ,opt=0;
            vector<vector<int>> temp;
            cout << "0. Decrease Size\n1. Increase Size\nEnter Choice:";
            cin >> opt;
            cout << "Enter the Resizing ratio of picture:\n";
            cout << "For row:";
            cin >> row;
            cout << "For columns:";
            cin >> col;
           
            if (opt == 0)
            {
                temp.resize(rows, vector<int>(cols));
                for (int r = 0, i = 0; r < rows; i++, r += row)
                {
                    for (int j = 0, c = 0; c < cols; c += col, j++)
                    {
                        temp[i][j] = ImageData[r][c];
                    }
                }
                for (int r = 0; r < rows; r++)
                    for (int c = 0; c < cols; c++)
                        ImageData[r][c] = maxGray;
                rows /= row;
                cols /= col;
                ImageData = temp;
            }
            else if (opt==1)
            {
                temp.resize(rows * row, vector<int>(cols * col,0));
                
                for (int i = 0; i < rows*row; i++)
                {
                    for (int j = 0; j < cols*col; j++)
                    {
                        if (j%col==0&&i%row==0)
                        {
                            temp[i][j] = ImageData[i / row][j / col];
                        }
                        else
                        {

                                temp[i][j] = ImageData[(i - i % row)/row][(j - j % col)/col];

                        }
                    }
                }
                
                rows *= row;
                cols *= col;
                ImageData.resize(rows, vector<int>(cols));
                ImageData = temp;
            }
        }

        void crop_it() {
            int sr = 0, sc = 0, er = rows, ec = cols ,row,col;
            vector<vector<int>>temp;
            cout << "Enter the starting row number to crop:";
            cin >> sr;
            cout << "Enter the ending row number you want to crop:";
            cin >> er;
            cout << "Enter the starting column number you want to crop:";
            cin >> sc;
            cout << "Enter the ending column number you want to crop:";
            cin >> ec;
            if (sr>rows)
            {
                sr = rows-1;
            }
            if (er>rows)
            {
                er = rows-1;
            }
            if (sc > cols)
            {
                sc = cols - 1;
            }
            if (ec > cols)
            {
                ec = cols - 1;
            }
            if (sr<er&&sc<ec)
            {
                row=er-sr+2, col=ec-sc+2;
                temp.resize(row, vector<int>(col));
                for (int r = sr,i=0; r <= er&&i<row;i++, r++)
                {
                    for (int c = sc,j=0; c <= ec&&j<col; j++,c++)
                    {
                        temp[i][j] = ImageData[r][c];
                    }
                }
                ImageData.resize(row, vector<int>(col));
                ImageData = temp;
                rows = row;
                cols = col;
            }
            
        }

        void scale_up() {
            int factor;
            cout << "Enter the factor by which you want to scale it up:";
            cin >> factor;
            vector<vector<int>> temp;
            temp.resize(rows * factor, vector<int>(cols * factor, 0));

            for (int i = 0; i < rows * factor; i++)
            {
                for (int j = 0; j < cols * factor; j++)
                {
                    if (j % factor == 0 && i % factor == 0)
                    {
                        temp[i][j] = ImageData[i / factor][j / factor];
                    }
                    else
                    {

                        temp[i][j] = ImageData[(i - i % factor) / factor][(j - j % factor) / factor];

                    }
                }
            }
            rows *= factor;
            cols *= factor;
            ImageData.resize(rows, vector<int>(cols));
            ImageData = temp;
        }

        void scale_down() {
            int factor;
            cout << "Enter the factor by which you want to scale it down:";
            cin >> factor;
            vector<vector<int>>temp;
            temp.resize(rows, vector<int>(cols));
            for (int r = 0, i = 0; r < rows; i++, r += factor)
            {
                for (int j = 0, c = 0; c < cols; c += factor, j++)
                {
                    temp[i][j] = ImageData[r][c];
                }
            }
            for (int r = 0; r < rows; r++)
                for (int c = 0; c < cols; c++)
                    ImageData[r][c] = maxGray;
            rows /= factor;
            cols /= factor;
            ImageData = temp;


        }

        void sort_it(int arr[],int  length) {

            for (int i = 0; i < length/2; i++)
            {
                for (int j = i; j < length; j++)
                {
                    if (arr[i]>arr[j])
                    {
                        swap(arr[i], arr[j]);
                    }
                }
            }

        }

        void Median_filter() {
            int arr[9] = { 0 };
            for (int r = 0; r < rows-3; r++)
            {
                for (int c = 0; c < cols-3; c++)
                {
                    for (int i = r, point = 0; i < (r+3); i++)
                    {
                        for (int j = c; j < (c+3); j++,point++)
                        {
                            arr[point] = ImageData[i][j];
                        }
                    }
                    sort_it(arr, 9);
                    ImageData[r][c] = arr[5];
                }
            }


        }

        void Mean_Filter() {
            int sum = 0;
            for (int r = 0; r < rows - 3; r++)
            {
                for (int c = 0; c < cols - 3; c++)
                {
                    sum = 0;
                    for (int i = r, point = 0; i < (r + 3); i++)
                    {
                        for (int j = c; j < (c + 3); j++, point++)
                        {
                            sum += ImageData[i][j];
                        }
                    }
                    ImageData[r][c] = sum / 9;
                }
            }
        }

        void SobelDerivativeImage() {
            int sobelHorizontal[3][3] = { {1, 0, -1},
                                 {2, 0, -2},
                                 {1, 0, -1} };

            int sobelVertical[3][3] = { {1, 2, 1},
                                       { 0,  0,  0},
                                       { -1,  -2,  -1} };


            for (int r = 0; r < rows-3; r++)
            {
                for (int c = 0; c < cols-3; c++)
                {
                    int Sumv = 0, Sumh = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            Sumh += sobelHorizontal[i][j] * ImageData[r + i][c + j];
                        }
                        for (int j = 0; j < 3; j++)
                        {
                            Sumv += sobelVertical[i][j] * ImageData[r + i][c + j];
                        }
                        ImageData[r][c] = sqrt(Sumh * Sumh + Sumv * Sumv);
                    }
                }
            }
            rows -= 3;
            cols -= 3;
        }

        void PrewittDerivativeImage() {
            int sobelHorizontal[3][3] = { {1, 0, -1},
                                 {1, 0, -1},
                                 {1, 0, -1} };

            int sobelVertical[3][3] = { {1, 1, 1},
                                       { 0,  0,  0},
                                       { -1,  -1,  -1} };


            for (int r = 0; r < rows - 3; r++)
            {
                for (int c = 0; c < cols - 3; c++)
                {
                    int Sumv = 0, Sumh = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            Sumh += sobelHorizontal[i][j] * ImageData[r + i][c + j];
                        }
                        for (int j = 0; j < 3; j++)
                        {
                            Sumv += sobelVertical[i][j] * ImageData[r + i][c + j];
                        }
                        Sumh /= 9;
                        Sumv /= 9;
                        ImageData[r][c] = sqrt(Sumh * Sumh + Sumv * Sumv);
                    }
                }
            }
            rows -= 3;
            cols -= 3;
        }

        void Side_by_side(vector<vector<int>> ImageData2,int rows2, int cols2) {
            int t = cols, tr=rows;
            cols += cols2;
            if (rows2>rows)
            {
                rows = rows2;
            }
            vector<vector<int>> temp(rows, vector<int>(cols,0));
            for (int r = 0; r < tr; r++)
            {
                for (int c = 0; c < t; c++)
                {
                    temp[r][c] = ImageData[r][c];
                }
            }
            for (int r = 0; r < rows; r++)
            {
                for (int c = t, j=0; c < cols && j<cols2; c++, j++)
                {
                    temp[r][c] = ImageData2[r][j];
                }
            }
            ImageData.resize(rows, vector<int>(cols));
            ImageData = temp;
        }

        void Top_to_Bottom(vector<vector<int>> ImageData2, int rows2, int cols2) {
            int t = rows, tc=cols;
            rows += rows2;
            if (cols2>cols)
            {
                cols = cols2;
            }
            vector<vector<int>> temp(rows, vector<int>(cols,0));
            for (int r = 0; r < t; r++)
            {
                for (int c = 0; c < tc; c++)
                {
                    temp[r][c] = ImageData[r][c];
                }
            }
            for (int r = t, i=0; r < rows&&i<rows2; r++,i++)
            {
                for (int c = 0; c < cols; c++)
                {
                    temp[r][c] = ImageData2[i][c];
                }
            }
            ImageData.resize(rows, vector<int>(cols));
            ImageData = temp;
        }

        void Linear_Stretching() {
            int min = maxGray, max = 0;
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
                    if (ImageData[r][c]>max)
                    {
                        max = ImageData[r][c];
                    }
                    if (ImageData[r][c]<min)
                    {
                        min = ImageData[r][c];
                    }
                }
            }
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
                    ImageData[r][c] -= min;
                    ImageData[r][c] *= 255; 
                    ImageData[r][c] /= max - min;
                }
            }
        }

        void Increase_Sharpness() {
            int Sharpen[3][3] = { {-1,-1,-1},
                                 {-1,9,-1},
                                 {-1, -1, -1} };

            int sum = 0;
            for (int r = 0; r < rows - 3; r++)
            {
                for (int c = 0; c < cols - 3; c++)
                {
                    sum = 0;
                    for (int i = r,y=0; i < (r + 3)&&y<3; i++,y++)
                    {
                        for (int j = c,x=0; j < (c + 3)&&x<3; j++,x++)
                        {
                            sum += ImageData[i][j]*Sharpen[y][x];
                        }
                    }
                    ImageData[r][c] = sum;
                }
            }

        }

        void Apply_from_file() {
            char FilterFileName[100];
            cout << "Specify Filter File Name ";
            cin >> FilterFileName;
            ifstream Filter(FilterFileName);
            int Fil[3][3] = {0};
            if (!Filter.is_open())
            {
                cout << "Filter not Found!\n";
                return;
            }
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Filter >> Fil[i][j];
                }
            }
            Filter.close();
            int sum = 0;
            for (int r = 0; r < rows - 3; r++)
            {
                for (int c = 0; c < cols - 3; c++)
                {
                    sum = 0;
                    for (int i = r, y = 0; i < (r + 3) && y < 3; i++, y++)
                    {
                        for (int j = c, x = 0; j < (c + 3) && x < 3; j++, x++)
                        {
                            sum += ImageData[i][j] * Fil[y][x];
                        }
                    }
                    ImageData[r][c] = sum;
                }
            }

        }
    };

    struct Menu {
        vector<string> menuItems;

        Menu(char menuFile[]) {
            loadMenu(menuFile);
        }

        int loadMenu(char menuFile[]) {
            ifstream IN;
            IN.open(menuFile);
            if (!IN.is_open())
                return -1;
            char menuItem[100], TotalItems[10];

            int Choices;

            IN.getline(TotalItems, 8);
            Choices = atoi(TotalItems);
            for (int i = 1; i <= Choices; i++) {
                IN.getline(menuItem, 99);
                menuItems.push_back(menuItem);
            }
            IN.close();
            return Choices;
        }

        int presentMenu() {
            int userChoice;
            int totalChoices = menuItems.size();


            do {
                int k = 1;
                for (int i = 0; i < totalChoices; i++) {
                    if (menuItems[i][0] != '*') {
                        cout << k << "\t" << menuItems[i] << endl;
                        k++;
                    }
                }
                cout << " Enter Your Choice (1 - " << k - 1 << " ) ";
                cin >> userChoice;
            } while (userChoice < 1 || userChoice > totalChoices);
            return userChoice;
        }

    };

    int main() {
        char MenuFile[] = "MainMenu.txt";
        Image images[2];
        int activeImage = 0;
        int errorCode = 0;
        int userChoice;
        int totalChoices;

        Menu menu(MenuFile);
        totalChoices = menu.menuItems.size();
        do {
            userChoice = menu.presentMenu();
            if (1 == userChoice) {
                char ImageFileName[100];
                cout << "Specify File Name ";
                cin >> ImageFileName;
                errorCode = images[activeImage].loadImage(ImageFileName);
                if (errorCode == 0) {
                    cout << "File Loaded Successfully " << endl;
                }
                else {
                    cout << "Load Error: Code " << errorCode << endl;
                }
            }
            else if (2 == userChoice) {
                char ImageFileName[100];
                cout << "Specify File Name ";
                cin >> ImageFileName;
                errorCode = images[activeImage].saveImage(ImageFileName);
                if (errorCode == 0) {
                    cout << "File Saved as " << ImageFileName << endl;
                }
                else {
                    cout << "Save Error: Code " << errorCode << endl;
                }
            }
            else if (3 == userChoice) {
                double factor;
                cout << "Enter the Brightness Factor:";
                cin >> factor;
                images[activeImage].changeBrightness(factor);
                cout << "You need to save the changes " << endl;
            }
            else if (4 == userChoice) {
                images[activeImage].Linear_Stretching();
                cout << "Linear Stretching has beed applied!\nYou need to Save Changes:) \n";
            }
            else if (5 == userChoice) {
                images[activeImage].Increase_Sharpness();
                cout << "Image is Sharpened!\nYou need to Save Changes:) \n";
            }
            else if (6 == userChoice) {
                images[activeImage].cnvrt_to_bin();
                cout << "Your Image is Converted into Binary\nYoue need to Save Changes:)\n";
            }
            else if (7 == userChoice) {
                images[activeImage].resize_by_ratio();
                cout << "Your Image Size is changed!\nYou need to save the changes:) " << endl;
            }
            else if (8 == userChoice) {
                int choice;
                cout << "0. 90 Degree clockwise\n1. 90 Degree anti clockwise\n2. Rotate by Theeta\nEnter Choice: ";
                cin >> choice;
                if (choice==0)
                {
                    images[activeImage].Rot_to_right();
                }
                if (choice==1)
                {
                    images[activeImage].rot_to_lef();
                }
                if (choice==2)
                {
                    images[activeImage].Rot_to_theeta();
                }
                cout << "Your Image is Rotated!\nYou need to save the changes:) " << endl;
            }
            else if (9 == userChoice || 20 == userChoice) {
                int choice;
                cout << "0. Vertical Flip\n1. Horizontl Flip\nEnter Choice:";
                cin >> choice;
                if (choice == 0)
                {
                    images[activeImage].vertical_flip();
                }
                if (choice == 1)
                {
                    images[activeImage].horzontalFlipImage();
                }
                cout << "Your Image is fliped!\nYou need to save the changes:) " << endl;
            }
            else if (10 == userChoice) {
                images[activeImage].crop_it();
                cout << "Your Image is croped!\nYou need to save the changes:) " << endl;
            }
            else if (12 == userChoice) {
                int choice;
                cout << "0. Mean Filter\n1. Median Filter\nEnter Choice: ";
                cin >> choice;
                if (choice == 0)
                {
                    images[activeImage].Mean_Filter();
                }
                if (choice == 1)
                {
                    images[activeImage].Median_filter();
                }
                cout << "Filter is Applied!\nYou need to save the changes:) " << endl;
            }
            else if (13 == userChoice) {
                images[activeImage].Apply_from_file();
                cout << "Filter is Applied!\nYoue need to save Changes:) \n";
            }
            else if (11 == userChoice) {
                char ImageFileName[100];
                cout << "Specify File Name of Second Picture ";
                cin >> ImageFileName;
                errorCode = images[1].loadImage(ImageFileName);
                if (errorCode == 0) {
                    cout << "File Loaded Successfully " << endl;
                }
                else {
                    cout << "Load Error: Code " << errorCode << endl;
                }
                int choice;
                cout << "0.Side by Side\n1.Top to Bottom\nEnter Choice:";
                cin >> choice;
                if (choice==0)
                {
                    images[0].Side_by_side(images[1].ImageData, images[1].rows, images[1].cols);
                }
                else if (choice==1)
                {
                    images[0].Top_to_Bottom(images[1].ImageData, images[1].rows, images[1].cols);
                }
                cout << "You need to save the changes " << endl;
            }
            else if (14 == userChoice) {
                images[activeImage].Linear_Stretching();
                cout << "Your Image is Inhanced!\nYou need to save the changes:) " << endl;
            }
            else if (15 == userChoice) {
                images[activeImage].PrewittDerivativeImage();
                cout << "Derivative Mask is Applied\nYou need to save the changes:) " << endl;
            }
            else if (16 == userChoice) {
                images[activeImage].SobelDerivativeImage();
                cout << "Image with edges is created!\nYou need to save the changes:) " << endl;
            }
            else if (17 == userChoice) {
                images[activeImage].crop_it();
                cout << "Your Image is Translated!\nYou need to save the changes:) " << endl;
            }
            else if (18 == userChoice) {
                int choice;
                cout << "0. Scale up\n1. Scale Down\nEnter Choice: ";
                cin >> choice;
                if (choice == 0)
                {
                    images[activeImage].scale_up();
                }
                if (choice == 1)
                {
                    images[activeImage].scale_down();
                }
                cout << "Image is Scaled!\nYou need to save the changes:) " << endl;
            }
            else if (19 == userChoice) {
                int choice;
                cout << "0. 90 Degree clockwise\n1. 90 Degree anti clockwise\n2. Rotate by Theeta\nEnter Choice: ";
                cin >> choice;
                if (choice == 0)
                {
                    images[activeImage].Rot_to_right();
                }
                if (choice == 1)
                {
                    images[activeImage].rot_to_lef();
                }
                if (choice == 2)
                {
                    images[activeImage].Rot_to_theeta();
                }
                cout << "Image is rotated!\nYou need to save the changes:) " << endl;
            }

        } while (userChoice != totalChoices);
        system("pause");
        return 0;
    }
