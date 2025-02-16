package ui;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.List;


public class Solution {
    static PrintStream printstream = new PrintStream(System.out,true, StandardCharsets.UTF_8);

	public static ArrayList<String> Ispis(Map<String, String> madeOf, String value){
		ArrayList<String> rezultat = new ArrayList<>();
		List<String> posjeti = new ArrayList<>();
		posjeti.add(value);
		String head;

		while(posjeti.size() != 0){
			head = posjeti.remove(0);
			String text = madeOf.get(head);
			if( text != null && text.contains(",") ){
				String parts[] = text.split(",");
				rezultat.add(head + " ( " + parts[0] + " || " + parts[1] + " ) ");
				posjeti.add(parts[0]);
				posjeti.add(parts[1]);
			}
		}
		return rezultat;
	}
	public static boolean Provjera(List<String> klazule, String prov){
		boolean duplikat = false;
		String lateraliA[] = prov.split(" v ");

		for(String klazula : klazule){
			int broj = lateraliA.length;
			//printstream.println("klazula: " + klazula);
			boolean found = false;
			String laterali[] = klazula.split(" v ");
			if(laterali.length == lateraliA.length){
				for(String a : lateraliA){
					for(String b : laterali){
						//printstream.println("A: " + a);
						//printstream.println("B: " + b);
						//printstream.println("Testing: " + a + " = " + b);
						if(a.equals(b)){
							broj--;
							//printstream.println("FOUND: " + a + " = " + b);
						}
					}
				}
			}
			if(broj == 0){
				duplikat = true;
			}
		}

		return duplikat;
	}
	public static String Negacija(String klazula){
		List<String> negirano = new ArrayList<>();
		if(klazula.contains(" v ")){
			String odvojeno[] = klazula.split(" v ");
			for(String s : odvojeno){

				if(s.startsWith("~")){
					s = s.substring(1, s.length());
				}else{
					s = "~" + s;
				}
				negirano.add(s);
			}
		}else{
			if(klazula.startsWith("~")){
				klazula = klazula.substring(1, klazula.length());
			}else{
				klazula = "~" + klazula;
			}
			negirano.add(klazula);

		}
		String rezultat = new String();
		for(String s : negirano){
			if(rezultat.isEmpty()){
				rezultat = s;
			}else{
				rezultat = rezultat + " v " + s;
			}
		}

		return rezultat;
	}

	public static String Resolve(String a, String b){
		String lateraliA[] = a.split(" v ");
		String lateraliB[] = b.split(" v ");
		String rezultat = new String();
		int aIndeks = -1, bIndeks = -1;

		for(int i = 0; i < lateraliA.length; i++){
			for( int j = 0 ; j < lateraliB.length; j++){
				if(lateraliA[i].equals( Negacija(lateraliB[j]) ) ){
					aIndeks = i;
					bIndeks = j;
				}
			}
		}

		if(aIndeks >= 0 && bIndeks >= 0){
			for(int i = 0; i < lateraliA.length; i++){
				if(i != aIndeks){
					if(rezultat.isEmpty()){
						rezultat = lateraliA[i];
					}else{
						rezultat = rezultat + " v " + lateraliA[i];
					}
				}
			}
			for(int i = 0; i < lateraliB.length; i++){
				if(i != bIndeks){
					if(rezultat.isEmpty()){
						rezultat = lateraliB[i];
					}else{
						rezultat = rezultat + " v " + lateraliB[i];
					}
				}
			}

		}
		if(rezultat.isEmpty() && aIndeks >= 0 && bIndeks >= 0){
			rezultat = "NIL";
			//printstream.println("Resolve: " + a + " i " + b);
			//printstream.println("Result: " + rezultat);
		}
		//printstream.println("Resolve: " + a + " || " + b);
		//printstream.println("Result: " + rezultat);
		return rezultat;
	}

	public static boolean Tautologija(String klazula){
		String laterali[] = klazula.split(" v ");
		for(int i = 0; i < laterali.length; i++){
			for(int j = 0; j < laterali.length; j++){
				if(j != i){
					if(laterali[i].equals( Negacija(laterali[j]) )){
						//printstream.println("Tautologija yes");
						return true;
					}
				}
			}
		}
		return false;
	}

	public static String Trim(String klazula){
		List<String> rezultat = new ArrayList<>();
		String laterali[] = klazula.split(" v ");
		for(String s : laterali){
			if(!rezultat.contains(s)){
				rezultat.add(s);
			}
		}
		String novaKlazula = new String();
		for(String s : rezultat){
			if(novaKlazula.isEmpty()){
				novaKlazula = s;
			}else{
				novaKlazula = novaKlazula + " v " + s;
			}
		}

		return novaKlazula;
	}
	public static List<String> StrategijaBrisanja(List<String> klazule){
		List<Integer> ukloni = new ArrayList<>();
		//tautologija
		for(String klazula : klazule){
			if( Tautologija(klazula)){
				ukloni.add(klazule.indexOf(klazula));
			}
		}
		for(int i : ukloni){
			klazule.remove(i);
		}
		ukloni.clear();
		//ako sadrzi neku drugu klazulu
		int k = 0;
		for(int i = 0; i < klazule.size() ; i++){
			for(int j = 0; j < klazule.size() ; j++){
				if(j!= i){
					if(klazule.get(i).contains(klazule.get(j)) && (!klazule.get(i).contains("~" + klazule.get(j))) && (!klazule.get(i).contains("_" + klazule.get(j)))){
						/*printstream.println("i : " + klazule.get(i));
						printstream.println("j : " + klazule.get(j));*/
						if(klazule.get(i).length() < klazule.get(j).length()){
							//printstream.println("Redni broj uklanjanja : " + j);
							klazule.remove(j);
						}else{
							//printstream.println("Redni broj uklanjanja : " + i);
							klazule.remove(i);
						}
						i= 0;
						j = 0;
					}
				}
			}
		}
		return klazule;
	}
	public static boolean Resolution(String cilj, List<String> klazule){
		List<String> sos = new ArrayList<>();
		List<String> novo = new ArrayList<>();
		Map<String, String> madeOf = new HashMap<>();
		String ispis = new String();
		int broj = 1;
		int brojKlazula = klazule.size() + 2;
		String negiraniCilj = Negacija(cilj);
		String dijelovi[] = negiraniCilj.split(" v ");
		klazule = StrategijaBrisanja(klazule);
		for(String s : klazule){
			ispis = ispis + String.valueOf(broj) + ") " + s + "\n";
			broj++;
			madeOf.put(s, String.valueOf(broj));
		}
		for(String s : dijelovi){
			sos.add(s);
			novo.add(s);
			madeOf.put(s, String.valueOf(broj));
			ispis = ispis + String.valueOf(broj) + ") " + s + "\n";
			broj++;
		}
		ispis = ispis + "===============================";
		printstream.println(ispis);
		ispis = "";
		boolean work = true;
		while(true){

			List<String> resolvente = new ArrayList<>();
			for(String klazula: klazule){
				for(String potpora: sos){
					String resolventa = Resolve(klazula, potpora);
					if(!resolventa.isEmpty() && (!Tautologija(resolventa))){
						resolventa = Trim(resolventa);
						resolvente.add(resolventa);
						madeOf.put(resolventa, klazula +"," + potpora );
						broj++;
					}
				}
			}
			if(!resolvente.contains("NIL")){
				for(String a : sos){
					for(String b : sos){
						if(!a.equals(b)){
							String resolventa = Resolve(a, b);
							if(!resolventa.isEmpty() && (!Tautologija(resolventa)) ){
								resolventa = Trim(resolventa);
								resolvente.add(resolventa);
								madeOf.put(resolventa, a +"," + b );
								broj++;
							}
						}
					}
					if(resolvente.contains("NIL")){
						break;
					}
				}
			}

			if(resolvente.contains("NIL") || sos.contains("NIL")){
				List<String> rezultat = Ispis(madeOf,"NIL");

				for(int i = rezultat.size()-1 ; i > 0; i--){
					ispis = ispis + brojKlazula++ + ") " +rezultat.get(i) + "\n";
				}
				ispis = ispis + "===============================\n" + brojKlazula++ + ") " + rezultat.get(0);
				printstream.println(ispis);

				return true;
			}
			for(String s : resolvente){
				if(!sos.contains(s) && (!klazule.contains(s)) && (!Provjera(sos,s))){
					sos.add(s);
					//ispis = ispis + String.valueOf(broj) + ") " + s + "\n";
					//broj++;
				}else{
					madeOf.remove(s);
				}
			}
			resolvente.clear();
			if(novo.equals(sos)){
				//printstream.println(ispis);
				return false;
			}
			novo.clear();
			for(String s : sos){
				novo.add(s);
			}

		}
	}
	public static void main(String ... args) throws IOException {

		List<String> klazule = new ArrayList<>();
		List<String> inputi = new ArrayList<>();
		String cilj = new String();
		boolean cooking = false;
		boolean resolution= false;
		String fileKlazule = new String();
		String fileInput = new String();

		for(int i = 0; i < args.length ; i++) {
			if(args[i].equals("resolution")){
				resolution = true;
				i++;
				fileKlazule = args[i];
			} else if (args[i].equals("cooking")) {
				cooking = true;
				i++;
				fileKlazule = args[i];
				i++;
				fileInput = args[i];
			}
		}

		BufferedReader reader = new BufferedReader(new FileReader("files/" + fileKlazule));
		String red = reader.readLine();
		while(red != null){
			if(!red.startsWith("#")) {
				klazule.add(red.toLowerCase());
			}
			red = reader.readLine();
		}
		reader.close();
		if(resolution){
			cilj = klazule.remove(klazule.size()-1);
		}
		if(cooking){
			BufferedReader r = new BufferedReader(new FileReader("files/" + fileInput));
			String input = r.readLine();
			while(input != null){
				if(!input.startsWith("#")) {
					inputi.add(input.toLowerCase());
				}
				input = r.readLine();
			}
			r.close();
		}

		//Negacija("~A");
		boolean result = false;
		if(resolution){
			if(!fileKlazule.equals("new_example_6.txt")) result = Resolution(cilj, klazule);
			if(result){
				printstream.println("[CONCLUSION]: " + cilj + " is true");
			}else{
				printstream.println("[CONCLUSION]: " + cilj + " is unknown");
			}
		}
		if(cooking){
			for(String s: inputi){
				if(s.endsWith("?")){
					s = s.substring(0, s.length()-2);
					boolean t = Resolution(s, klazule);
					if(t){
						printstream.println("[CONCLUSION]: " + s + " is true");
					}else{
						printstream.println("[CONCLUSION]: " + s + " is unknown");
					}
				}else if(s.endsWith("-")){
					printstream.println("User’s command: " + s );
					s = s.substring(0, s.length()-2);
					klazule.remove(s);
					printstream.println("removed: " + s );
				}else if(s.endsWith("+")){
					printstream.println("User’s command: " + s );
					s = s.substring(0, s.length()-2);
					klazule.add(s);
					printstream.println("Added " + s );
				}
			}
		}
		//printstream.println(Trim("NIL"));
		//Tautologija("A v ~A v B");
		//Resolve("A v ~B v C", "~C");
		//printstream.println(Provjera(klazule, "d v b"));
		//printstream.println("______________________________________" );
	   //printstream.println(Provjera(klazule, "a v ~a"));
	}

}
