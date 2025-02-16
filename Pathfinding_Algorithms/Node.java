package ui;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

public class Node {
	public String state;
	public float heuristic;
	public float cost;
	public TreeMap<String, Float> prijelazi;
	public List<String> succesors;
	public String path;
	public int path_length;
	
	Node(String state, float cost, float heuristic){
		this.state = state;
		this.heuristic = heuristic;
		this.prijelazi = new TreeMap<String,Float>();
		this.succesors =  new ArrayList<String>();
	}
	Node(){
		this.state = null;
		this.heuristic = 0;
		this.prijelazi = new TreeMap<String,Float>();
		this.succesors = new ArrayList<String>();
		this.path= new String();
		this.cost = 0;
		this.path_length = 0;
	}
	
	public void Put(String name, float cost) {
		prijelazi.put(name, cost);
		succesors.add(name);
		return;
	}
	
	public int compare(Node node1, Node node2) {
		if (node1.cost < node2.cost)
            return -1;
        if (node1.cost > node2.cost)
            return 1;
        return 0;
	}
	
}
