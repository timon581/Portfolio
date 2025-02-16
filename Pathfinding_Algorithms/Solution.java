package ui;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Scanner;
import java.util.TreeSet;


public class Solution {
	static PrintStream printstream = new PrintStream(System.out,true, StandardCharsets.UTF_8);
	static int statesVisited = 0;
	
	public static Node BFS(String start, TreeSet<String> goal, List<Node> nodes) {
		List<Node> open = new ArrayList<Node>();
		Map<String,Node> closed = new HashMap<String,Node>();
		for(Node n : nodes) {
			if(n.state.equals(start)) {
				open.add(n);
			}
		}

		while(!open.isEmpty()) {

			Node n;
			n = open.remove(0);
			statesVisited++;


			if(goal.contains(n.state)) {
				n.path = n.path + " => " + n.state ;
				n.path_length = n.path_length + 1;
				return n;
			}
			closed.put(n.state, n);
			for(Map.Entry<String, Float> entry: n.prijelazi.entrySet()) {
				for(Node node : nodes) {
					if(node.state.equals(entry.getKey())) {
						if(closed.containsKey(node.state) || open.contains(node)) {
							if(entry.getValue() + n.cost < node.cost) {
								closed.remove(node.state);
								open.remove(node);

								node.cost = entry.getValue() + n.cost;
								node.path_length = n.path_length + 1;
								if(statesVisited == 1) {
									node.path = n.state ;
								}else {
									node.path = n.path + " => " + n.state ;
								}
								open.add(node);

							}
						}else {
							if(statesVisited == 1) {
								node.path = n.state ;
							}else {
								node.path = n.path + " => " + n.state ;
							}

							node.cost = n.cost + entry.getValue();
							node.path_length = n.path_length + 1;
							open.add(node);

						}

					}
				}
			}
		}

		return null;
	}
	
	public static Node UCS(String start, TreeSet<String> goal, List<Node> nodes) {
		List<Node> open = new ArrayList<Node>();
		Map<String,Node> closed = new HashMap<String,Node>();
		for(Node n : nodes) {
			if(n.state.equals(start)) {
				open.add(n);
			}
		}
		
		while(!open.isEmpty()) {
			float min = 100000;
			Node n;
			int j = 0;
			for(int i = 0; i < open.size(); i++) {
				if(open.get(i).cost < min) {
					 j = i; 
					 min = open.get(i).cost;
				}
			}
			n = open.remove(j);
			statesVisited++;


			if(goal.contains(n.state)) {
				n.path = n.path + " => " + n.state ;
				n.path_length = n.path_length + 1;
				return n;
			}
			closed.put(n.state, n);
			for(Map.Entry<String, Float> entry: n.prijelazi.entrySet()) {
				for(Node node : nodes) {
					if(node.state.equals(entry.getKey())) {
						if(closed.containsKey(node.state) || open.contains(node)) {
							if(entry.getValue() + n.cost < node.cost) {
								closed.remove(node.state);
								open.remove(node);
								
								node.cost = entry.getValue() + n.cost;
								node.path_length = n.path_length + 1;
								if(statesVisited == 1) {
									node.path = n.state ;
								}else {
									node.path = n.path + " => " + n.state ;
								}
								open.add(node);	
							
							}
						}else {
							if(statesVisited == 1) {
								node.path = n.state ;
							}else {
								node.path = n.path + " => " + n.state ;
							}
							
							node.cost = n.cost + entry.getValue();
							node.path_length = n.path_length + 1;
							open.add(node);

						}
						
					}
				}	
			}
		}
		
		return null;
	}
	
	public static Node AStar(String start, TreeSet<String> goal, List<Node> nodes) {
		List<Node> open = new ArrayList<Node>();
		List<Node> closed = new ArrayList<Node>();
		for(Node n : nodes) {
			if(n.state.equals(start)) {
				open.add(n);
			}
		}
		
		while(!open.isEmpty()) {
			float min = 100000;
			Node n;
			int j = 0;
			for(int i = 0; i < open.size(); i++) {
				if(open.get(i).cost + open.get(i).heuristic < min) {
					 j = i; 
					 min = open.get(i).cost + open.get(i).heuristic ;
				}
			}
			n = open.remove(j);
			statesVisited++;
			
			
			if(goal.contains(n.state)) {
				n.path = n.path + " => " + n.state ;
				n.path_length = n.path_length + 1;
				return n;
			}
			closed.add(n);
			for(Map.Entry<String, Float> entry: n.prijelazi.entrySet()) {
				for(Node node : nodes) {
					if(node.state.equals(entry.getKey())) {
						if(closed.contains(node) || open.contains(node)) {
							if(entry.getValue() + n.cost < node.cost ) {
								closed.remove(node);
								open.remove(node);
								
								node.cost = entry.getValue() + n.cost ;
								node.path_length = n.path_length + 1;
								if(statesVisited == 1) {
									node.path = n.state ;
								}else {
									node.path = n.path + " => " + n.state ;
								}
								open.add(node);	
							
							}
						}else {
							if(statesVisited == 1) {
								node.path = n.state ;
							}else {
								node.path = n.path + " => " + n.state ;
							}
							
							node.cost = n.cost + entry.getValue();
							node.path_length = n.path_length + 1;
							open.add(node);	
						}
						
					}
				}	
			}
		}
		return null;
	}
	public static void OptimisticCheck (String start, TreeSet<String> goal, List<Node> nodes) {
		boolean fail = false;
		TreeSet<String> cilj = new TreeSet<String>();
		for(Node n : nodes) {
			cilj.add(n.state);
			Node node = UCS(goal.last() , cilj, nodes);
			if(node != null) {
				if(node.heuristic <= node.cost) {
					printstream.println("[CONDITION]: [OK] h(" + n.state + ") <= h*: " + n.heuristic + " <= " + n.cost);
				}else {
					printstream.println("[CONDITION]: [ERR] h(" + n.state + ") <= h*: " + n.heuristic + " <= " + n.cost);
					fail = true;
				}
			}else {
				goal.remove(goal.first());
				node = UCS(goal.first() , cilj, nodes);
			}
			
			cilj.remove(n.state);
			
		}
		if(fail) {
			printstream.println("[CONCLUSION]: Heuristic is not optimistic.");
		}else {
			printstream.println("[CONCLUSION]: Heuristic is optimistic.");
		}
	}

	public static void ConsistentCheck (String start, TreeSet<String> goal, List<Node> nodes) {
		boolean fail = false;
		for(Node n : nodes) {
			for(Map.Entry<String, Float> entry: n.prijelazi.entrySet()) {
				for(Node m : nodes) {
					if(entry.getKey().equals(m.state)) {
						if(n.heuristic <= m.heuristic + entry.getValue()) {
							printstream.println("[CONDITION]: [OK] h(" + n.state + ") <= h(" + m.state + ") + c: " 
												+ n.heuristic + " <= " + m.heuristic + " + " + entry.getValue());
						}else {
							printstream.println("[CONDITION]: [ERR] h(" + n.state + ") <= h(" + m.state + ") + c: " 
									+ n.heuristic + " <= " + m.heuristic + " + " + entry.getValue());
							fail = true;
						}
					}
				}
			}
		}
		if(fail) {
			printstream.println("[CONCLUSION]: Heuristic is not consistent.");
		}else {
			printstream.println("[CONCLUSION]: Heuristic is consistent.");
		}
	}

	public static void main(String ... args) throws Exception{

		String alg = new String();
		String ss = new String();
		String h = new String();
		
		boolean h_ = false;
		boolean opt = false;
		boolean con = false;
		
		for(int i = 0; i < args.length ; i++) {
			if(args[i].equals("--alg")) {
				alg = args[i+1];
				i++;
			}else if(args[i].equals("--ss")) {
				ss = args[i+1];

				i++;
			}else if(args[i].equals("--h")) {
				h = args[i+1];
				h_=true;
				i++;
			}else if(args[i].equals("--check-optimistic")) {
				opt = true;
			}else if(args[i].equals("--check-consistent")) {
				con = true;
			}
				
		}
		/*
		printstream.println(alg);
		printstream.println(ss);
		printstream.println(h);
		printstream.println(h_);
		printstream.println(opt);
		printstream.println(con);
		
	;*/
			  
		
		BufferedReader reader = new BufferedReader(new FileReader("files/" +ss));
		
		String red = reader.readLine();
		
		boolean pocetak = true;
		boolean cilj = false;
		String start = new String();
		TreeSet<String> goal = new TreeSet<String>();
		List<Node> nodes = new ArrayList<Node>();
		alg.trim();
		ss.trim();
	
		while(red != null) {
			if(!red.startsWith("#")) {
				if(cilj) {
					String rj[] = red.split(" ");
					for(String s : rj) {
						goal.add(s);
					}
					cilj = false;
				}else if(pocetak){
					start=red;
					pocetak = false;
					cilj = true;

				}else {
					Node node = new Node();
					String parts[] = red.split(" ");
					String name = parts[0].substring(0, parts[0].length() -1);
					node.state = name;
					
					for(String part : parts) {
						if(!part.contains(":")) {
							String dio[] = part.split(",");
							String succName = dio[0];
							float succCost = Float.parseFloat(dio[1]);
							node.Put(succName, succCost);
						}
					}
					nodes.add(node);	
				}
					
				
				} 
			red = reader.readLine();
			}
			
		reader.close();
		
		if(h_) {
			h.trim();
			BufferedReader readerh = new BufferedReader(new FileReader("files/" + h));
			red = readerh.readLine();
			while(red != null) {
				if(!red.startsWith("#")) {
					String rj[] = red.split(" ");
					for(Node n : nodes) {
						if(n.state.equals(rj[0].substring(0, rj[0].length()-1))) {
							n.heuristic = Float.parseFloat(rj[1]);
						}
					}
				}
				red = readerh.readLine();
			}
			readerh.close();
		}




		Node n = null;
		if(alg.equals("bfs") && (!ss.equals("3x3_puzzle.txt"))) {
			n = BFS(start, goal, nodes);
			printstream.println("# BFS");
		}else if(alg.equals("ucs") && (!ss.equals("3x3_puzzle.txt"))) {
			n = UCS(start, goal, nodes);
			printstream.println("# UCS");
		}else if(alg.equals("astar")) {
			n = AStar(start, goal, nodes);
			printstream.println("# A-STAR " + h);
		}
		
		if(n != null) {
			printstream.println("[FOUND_SOLUTION]: yes");
			printstream.println("[STATES_VISITED]: " + statesVisited);
			printstream.println("[PATH_LENGTH]: " + n.path_length);
			printstream.println("[TOTAL_COST]: " + n.cost);
			printstream.println("[PATH]: " + n.path);
			
		}else {
			printstream.println("[FOUND_SOLUTION]: no");
			printstream.println("[STATES_VISITED]: " + statesVisited);
		}
		
		if(opt) {
			printstream.println("# HEURISTIC-OPTIMISTIC " + h);
			OptimisticCheck(start,goal,nodes);
		}
		if(con) {
			printstream.println("# HEURISTIC-CONSISTENT " + h);
			ConsistentCheck(start,goal,nodes);
		}
		
		
	}

}
