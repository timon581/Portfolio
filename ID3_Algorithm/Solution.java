package ui;

import com.sun.source.tree.Tree;

import java.io.*;
import java.math.RoundingMode;
import java.nio.charset.StandardCharsets;
import java.util.*;

class Node{
	static PrintStream printstream = new PrintStream(System.out,true, StandardCharsets.UTF_8);
	List<Node> children;
	List<String> transitions;
	String value;
	String atribute;
	int atributeIndeks;
	TreeSet<String> atributeValues;
	String common;

	Node(String s){
		value = s;
		children = null;
	}
	Node(List<Node> children,List<String> transitions, String atribute, TreeSet<String> atributeValues, int atributeIndeks, String common){
		this.children = children;
		this.atribute = atribute;
		this.atributeValues = atributeValues;
		this.atributeIndeks = atributeIndeks;
		this.transitions = transitions;
		this.common = common;
		value = null;
	}

	public boolean hasChildren(){
		if(children == null) return false;
		else return true;
	}
	public boolean isLeaf(){
		if(value == null)return false;
		else return true;
	}

}
public class Solution {

	public static Node ID3Depth(List<String> set, List<String> parentSet, int numberatributes, List<String> atributes, HashMap<String,Integer> atributeIndekses, int depth, int generated){
		if(set.isEmpty()) {
			String v = MostCommon(parentSet);
			Node n = new Node(v);
			//printstream.println(v);
			return n;
		}
		List<String> setV = new ArrayList<>();
		MostCommon(set,setV);
		String v = MostCommon(set);
		if(atributes.isEmpty() || set.equals(setV) || generated == depth){
			Node n = new Node(v);
			//printstream.println(v);
			return n;
		}
		int argmax = 0;
		double max_information = 0;
		double entropy = EntropyLabel(set);
		TreeSet<Double> IG = new TreeSet<>();
		//printstream.println("Entropy : " +  entropy);
		for(int i = 0; i < atributes.size(); i++){
			double ig = InformationGain(set, i, entropy);
			double igRounded =  Math.round(ig * 10000.0) / 10000.0;
			//printstream.print("IG(" + atributes.get(i) + ") = " + igRounded + " ");
			IG.add(igRounded);
			if(ig > max_information){
				argmax = i;
				max_information = ig;
			}
		}
		printstream.print(IG);
		printstream.println();
		List<Node> subtress = new ArrayList<>();
		List<String> transitions = new ArrayList<>();
		TreeSet<String> values = Values(set, argmax);
		String removedAtribute = atributes.remove(argmax);
		//printstream.println("Removed atribute: " + removedAtribute);
		//printstream.println("Atributes: " + atributes + "numberofatributes: " + (numberatributes-1));
		numberatributes = numberatributes -1 ;
		for(String value : values){
			List<String> childSet = ExtractByValue(set, value, argmax);
			if(generated < depth){
				List<String> noviAtributes = new ArrayList<>(atributes);
				Node n = ID3Depth(childSet, set, numberatributes, noviAtributes, atributeIndekses,depth, generated + 1);
				subtress.add(n);
				transitions.add(value);
			}
		}
		Node root = new Node(subtress,transitions, removedAtribute, values, atributeIndekses.get(removedAtribute), v);
		return root;
	}
	public static Node ID3(List<String> set, List<String> parentSet, int numberatributes, List<String> atributes, HashMap<String,Integer> atributeIndekses){
		if(set.isEmpty()) {
			String v = MostCommon(parentSet);
			Node n = new Node(v);
			//printstream.println(v);
			return n;
		}
		List<String> setV = new ArrayList<>();
		MostCommon(set,setV);
		String v = MostCommon(set);
		if(atributes.isEmpty() || set.equals(setV)){
			Node n = new Node(v);
			//printstream.println(v);
			return n;
		}
		int argmax = 0;
		double max_information = 0;
		double entropy = EntropyLabel(set);
		//printstream.println("Entropy : " +  entropy);
		List<Double> IG = new ArrayList<>();
		for(int i = 0; i < atributes.size(); i++){
			double ig = InformationGain(set, i, entropy);
			double igRounded =  Math.round(ig * 10000.0) / 10000.0;
			//printstream.print("IG(" + atributes.get(i) + ") = " + igRounded + " ");
			IG.add(igRounded);
			if(ig > max_information){
				argmax = i;
				max_information = ig;
			}
		}
		printstream.print((IG));
		printstream.println();
		List<Node> subtress = new ArrayList<>();
		List<String> transitions = new ArrayList<>();
		TreeSet<String> values = Values(set, argmax);
		String removedAtribute = atributes.remove(argmax);
		for(String value : values){
			List<String> childSet = ExtractByValue(set, value, argmax);
			List<String> noviAtributes = new ArrayList<>(atributes);
			Node n = ID3(childSet, set, numberatributes - 1, noviAtributes, atributeIndekses);
			subtress.add(n);
			transitions.add(value);
		}
		Node root = new Node(subtress,transitions, removedAtribute, values, atributeIndekses.get(removedAtribute), v);
		return root;
	}

	public static void Branches(Node node, String branch, int depth){
		if(!node.hasChildren()){
			branch = branch + node.value;
			printstream.println(branch);
			return;
		}else{
			List<String> values = new ArrayList<>(node.atributeValues);
			String oldbranch = branch;
			for(int i = 0; i < node.transitions.size(); i++) {
				branch = oldbranch;
				branch = branch + depth + ":" + node.atribute + "=" + node.transitions.get(i) + " ";
				Branches(node.children.get(i),branch, depth + 1);
			}
		}
		//printstream.println(values);
	}

	public static String Prediction(Node node, String prediction, String statment){
		if(!node.hasChildren()){
			prediction = node.value;
			return prediction;
		}else {
			String parts[] = statment.split(",");
			List<String> values = new ArrayList<>(node.atributeValues);

			for (String s : parts) {
				//String t = parts[node.atributeIndeks];
				for(int i = 0; i < node.atributeValues.size(); i++){
					if(s.equals(values.get(i))){
						prediction = Prediction(node.children.get(i), prediction, statment);
					}
				}
			}
		}

		return prediction;

	}

	public static String Predict(Node node, String statement, int depth){
		//printstream.println("Depth: " + depth);
		//printstream.println("Node: " + node.atribute + ", Tranzitions: " + node.transitions );
		if(node.isLeaf()){
			//printstream.println("Reached leaf with value : " + node.value);
			return node.value;
		}else{
			int atributeIndeks = node.atributeIndeks;
			String parts[] = statement.split(",");
			String relevantAtribute = parts[atributeIndeks];
			for(int i = 0; i < node.transitions.size(); i++){
				if(relevantAtribute.equals(node.transitions.get(i))){
					//printstream.println(("Relevant atribute: " + relevantAtribute +", Chosen node: " + node.children.get(i).atribute));
					String result = Predict(node.children.get(i), statement, depth + 1);
					return result;
				}
			}
		}
		return node.common;
	}


	public static double customLog(double base, double logNumber) {
		double log = Math.log(logNumber) / Math.log(base);
		//printstream.println(log);
		return log;
	}

	public static List<String> ExtractByValue(List<String> set, String value, int n){
		List<String> newSet = new ArrayList<>();
		for(String s : set){
			String parts[] = s.split(",");
			if(parts[n].equals(value)) {
				String newS = RemoveAtribute(s,n);
				newSet.add(newS);
			}

		}
		return newSet;
	}

	public static String RemoveAtribute(String s, int n){
		String parts[] = s.split(",");
		String newS = new String();
		for(int i = 0; i < parts.length; i++){
			if( i != n){
				newS = newS + parts[i] + ",";
			}
		}
		newS = newS.substring(0, newS.length()-1);
		return newS;
	}
	public static TreeSet<String> Values(List<String> set, int n){
		TreeSet<String> values = new TreeSet<>();
		for(String s : set){
			String parts[] = s.split(",");
			values.add(parts[n]);
		}
		return values;
	}

	public static double Entropy(ArrayList<Double> probabilities){
		double entropy = 0;
		for(double n : probabilities) {
			//printstream.println(n);
			if (n != 0) {
				entropy = entropy -(n * customLog(2, n));
			}
		}
		return entropy;
	}

	public static double EntropyLabel(List<String> set){
		HashMap<String,Integer> values = new HashMap<>();
		double total = 0;
		for(String s : set){
			String parts[] = s.split(",");
			if(values.get(parts[parts.length-1]) == null){
				values.put(parts[parts.length-1], 1);
				total++;
			}else{
				int n = values.get(parts[parts.length-1]);
				values.put(parts[parts.length-1], n+1);
				total++;
			}
		}
		ArrayList<Double> probabilities = new ArrayList<>();
		for(Map.Entry entry : values.entrySet()){
			int v = (int) entry.getValue();
			double probability =(double) v / total;
			probabilities.add(probability);
		}
		double entropy = Entropy(probabilities);
		//printstream.println(entropy);
		return entropy;
	}

	public static String MostCommon(List<String> set){
		HashMap<String,Integer> values = new HashMap<>();
		for(String s : set){
			String parts[] = s.split(",");
			if(values.get(parts[parts.length-1]) == null){
				values.put(parts[parts.length-1], 1);
			}else{
				int n = values.get(parts[parts.length-1]);
				values.put(parts[parts.length-1], n+1);
			}
		}
		int max = 0;
		String maxString = new String();
		for(Map.Entry entry : values.entrySet()){
			int v = (int) entry.getValue();
			String s = (String) entry.getKey();
			if(v == max){
				int comp = s.compareTo(maxString);
				//printstream.println("Comparing " + s + " " + maxString + " " + s.compareTo(maxString));
				if(comp <= -1){
					max = v;
					maxString = s;
				}
			}
			if(v>max){
				max = v;
				maxString = s;
			}
		}
		return maxString;
	}
	public static List<String> MostCommon(List<String> set, List<String> newSet){
		String most = MostCommon(set);
		for(String s : set){
			String parts[] = s.split(",");
			String label = parts[parts.length-1];
			if(label.equals(most)) newSet.add(s);
		}
		return newSet;
	}
	public static double InformationGain(List<String> set, int n, double entropy){
		//TreeSet<String> values = new TreeSet<>();
		HashMap<String, Integer> values = new HashMap<>();
		double total = 0;
		int t = set.size();
		for(String s : set){
			String parts[] = s.split(",");
			if(values.get(parts[n]) == null){
				values.put(parts[n], 1);
				total++;
			}else{
				int v = values.get(parts[n]);
				values.put(parts[n], v+1);
				total++;
			}
		}

		for(Map.Entry entry : values.entrySet()){
			int v = (int) entry.getValue();
			String y = (String) entry.getKey();
			double probability =(double) v / total;
			//probabilities.add(probability);
			//TreeSet<Double> probabilities = new TreeSet<>();
			//HashMap<String, Integer> labels = new HashMap<>();
			List<String> setValue = new ArrayList<>();
			for(String s : set){
				String dijelovi[] = s.split(",");
				if(dijelovi[n].equals(y)){
					setValue.add(s);
				}
			}
			double atributeEntropy = EntropyLabel(setValue);
			entropy = entropy - (probability * atributeEntropy);

		}
		//printstream.println(value + " |yes:" + yes + " |no:" + no + " |ENtropy " + (atributeEntropy));
		return entropy;
	}
	static PrintStream printstream = new PrintStream(System.out,true, StandardCharsets.UTF_8);
	public static void main(String ... args) throws IOException {
		String fileTraining = new String();
		String filePrediction = new String();
		String header = new String();
		int parameters = 0;
		List<String> training = new ArrayList<>();
		List<String> prediciton = new ArrayList<>();
		List<String> atributes = new ArrayList<>();

		fileTraining = args[0];
		filePrediction = args[1];

		BufferedReader reader = new BufferedReader(new FileReader("files/" + fileTraining));
		String red = reader.readLine();
		header = red;
		parameters = header.split(",").length - 1;
		red = reader.readLine();
		while(red != null){
			training.add(red);
			red = reader.readLine();
		}
		reader.close();
		String head[] = header.split(",");
		for(int i = 0; i < head.length-1; i++){
			atributes.add(head[i]);
		}

		BufferedReader reader2 = new BufferedReader(new FileReader("files/" + filePrediction));
		red = reader2.readLine();
		red = reader2.readLine();
		while(red != null){
			prediciton.add(red);
			red = reader2.readLine();
		}
		reader2.close();

		HashMap<String, Integer> atributeIndekses = new HashMap<>();
		for(int i = 0; i < atributes.size();i++){
			atributeIndekses.put(atributes.get(i), i);
		}
		Node root;
		if(args.length > 2){
			int depth = Integer.valueOf(args[2]);
			root = ID3Depth(training, training, parameters, atributes, atributeIndekses, depth, 0);
		}else{
			root = ID3(training, training, parameters, atributes, atributeIndekses);
		}

		printstream.println("[BRANCHES]:");
		String branch = new String();
		Branches(root, branch, 1);
		printstream.print("[PREDICTIONS]: ");
		TreeSet<String> y = new TreeSet<>();
		TreeSet<String> p = new TreeSet<>(prediciton);
		List<String> predicted = new ArrayList<>();
		List<String> truth = new ArrayList<>();

		y = Values(prediciton, parameters);
		HashMap<String, Integer> hash = new HashMap<>();
		double correct = 0;

		for(int i = 0; i < prediciton.size(); i++){
			String result = new String();
			String parts[] = prediciton.get(i).split(",");
			String label = parts[parts.length-1];
			//String conducted = Prediction(root, result, prediciton.get(i));
			String toPredict = prediciton.get(i);
			String conducted = Predict(root, toPredict,1);
			predicted.add(conducted);
			truth.add(label);
			if(conducted.equals(label)) {
				correct++;
				if(hash.get(label) == null){
					hash.put(label, 1);
				}else{
					int n = (int) hash.get(label);
					hash.put(label, n+1);
				}
			}
			//printstream.println(prediciton.get(i) + " ");
			printstream.print(conducted + " ");
		}
		printstream.println();
		double accuracy = (double) (correct) / prediciton.size();
		double rounded = Math.round(accuracy * 100000.0) / 100000.0;
		String ispis = String.valueOf(rounded);
		String split[] = ispis.split("\\.");
		String cijeliDio = split[0];
		String decimalniDio = split[1];
		if(decimalniDio.length() < 5){
			int nule = 5 - decimalniDio.length();
			for(int i = 0; i < nule; i++){
				decimalniDio = decimalniDio.concat("0");
			}
		}
		printstream.println("[ACCURACY]: " + cijeliDio +"." +decimalniDio);
		printstream.println("[CONFUSION_MATRIX]: ");
		TreeSet<String> l = new TreeSet<>(prediciton);

		List<String> yList = new ArrayList<>(y);
		for(int i = 0; i < yList.size() ; i++){
			for(int j = 0; j < yList.size(); j++){
				int n = 0;
				String a = yList.get(i);
				String b = yList.get(j);
				for(int k = 0; k< predicted.size();k++){
					if(predicted.get(k).equals(b) && truth.get(k).equals(a)){
						n++;
					}
				}
				printstream.print(n + " ");
			}
			printstream.println();
		}


	}

}
