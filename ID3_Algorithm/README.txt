This project implements the ID3 decision tree algorithm in Java. The ID3 algorithm is used for classification problems by building a decision tree based on entropy and information gain.

Features:

	1) Builds a decision tree from a dataset.
	2) Supports depth-limited trees using ID3Depth.
	3) Predicts the classification of new data points.
	4) Prints the tree structure.
	5) Evaluates accuracy using a confusion matrix.

Compile code: javac Main.java
Run code: java Main train.csv test.csv

Replace train.csv with your training dataset.
Replace test.csv with your test dataset.

Input (CSV format) example: 
Outlook,Temperature,Humidity,Wind,PlayTennis
Sunny,Hot,High,Weak,No
Sunny,Hot,High,Strong,No
Overcast,Hot,High,Weak,Yes

Output:
Decision Tree:
[Outlook]
  - Sunny -> [Humidity]
    - High -> No
    - Normal -> Yes
  - Overcast -> Yes
  - Rain -> [Wind]
    - Weak -> Yes
    - Strong -> No

Accuracy: 85%
Confusion Matrix:
   Actual -> Predicted
   Yes  No
Yes  7   1
No   2   6

Train and test files used are in files folder.