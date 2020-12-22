use std::collections::{HashMap, HashSet};
use std::io::{self, Read};

fn main() -> io::Result<()> {
    let mut buf = String::new();
    io::stdin().read_to_string(&mut buf)?;
    let lines = buf.split('\n');

    // serialize input
    let mut pairs: Vec<(HashSet<String>, HashSet<String>)> = Default::default();
    for line in lines {
        let mut ingredients: HashSet<String> = Default::default();
        let mut allergens: HashSet<String> = Default::default();

        let mut parts = line.split("(contains ");

        parts.next().unwrap().split(" ").for_each(|ingredient| {
            ingredients.insert(ingredient.to_owned());
        });

        if let Some(a) = parts.next() {
            a.split(", ").for_each(|allergen| {
                let mut allergen = allergen.to_owned();
                if allergen.contains(")") {
                    allergen.pop();
                }
                allergens.insert(allergen);
            });
        };

        if !ingredients.is_empty() && !allergens.is_empty() {
            pairs.push((ingredients, allergens))
        }
    }

    // Create a global set of ingredients
    let mut ingredients: HashSet<String> = Default::default();
    // Mutatis mutandis allergens
    let mut allergens: HashSet<String> = Default::default();
    for (i, a) in pairs.iter() {
        ingredients = ingredients.union(&i).map(|x| x.to_owned()).collect();
        allergens = allergens.union(&a).map(|x| x.to_owned()).collect();
    }

    // counts which ingredient has occurred how often - which is required
    // for the final answer.
    let mut ingredient_occurrences: HashMap<String, u64> = Default::default();

    // Let's assume each ingredient contains every allergen - an obviously false
    // assumption, but we will strategically discard allergens for ingredients
    // in later steps.
    //
    // All ingredients with an empty value (empty set) in the following hashmap,
    // are ingredients that cannot be allergens.
    let mut allergen_possibilities: HashMap<String, HashSet<String>> = HashMap::new();
    for i in ingredients.iter() {
        allergen_possibilities.insert(i.to_owned(), allergens.clone());
    }

    for (i, a) in pairs.iter() {
        // count occurrences with this loop
        for ing in i.iter() {
            *ingredient_occurrences.entry(ing.to_string()).or_insert(0) += 1;
        }

        for al in a.iter() {
            for ing in ingredients.iter() {
                // if a particular food does not contain a global ingredient
                // then this global ingredient cannot contain the allergen
                // corresponding to this food.
                if !i.contains(&ing[..]) {
                    let set = allergen_possibilities.get_mut(ing).unwrap();
                    set.remove(al);
                }
            }
        }
    }

    // Finally, as described above, compute the non-allergic ingredients.
    let mut sum = 0;
    for (i, a) in allergen_possibilities.iter() {
        if a.is_empty() {
            sum += ingredient_occurrences[i];
        }
    }

    println!("Part one answer: {}", sum);

    // at this point, we know what ingredients do not have allergens, but we do not
    // know, for ingredients _with_ allergens, which allergens they contain.
    // However, we have a set of possibilities.
    //
    // We try to find an ingredient with only a single possiblity for an allergen,
    // and then remove that allergen from consideration. We keep repeating until
    // all ingredients have an allergen assigned to them.

    // This set holds already matched allergens. When this set contains all
    // allergens, we will be done.
    let mut matched_allergens = HashSet::new();

    // maps 1 ingredient to 1 allergen.
    let mut matching: HashMap<String, String> = HashMap::new();

    while matching.len() != allergens.len() {
        // we have to repeat the matching process for every unmatched ingredient.
        for ing in ingredients.iter() {
            let options: Vec<String> = allergen_possibilities
                .get(ing)
                .unwrap()
                .iter()
                .filter_map(|a| {
                    if !matched_allergens.contains(a) {
                        Some(a.to_owned())
                    } else {
                        None
                    }
                })
                .collect();

            if options.len() == 1 {
                let option = &options[0];

                // found unmatched allergen, assign to this ingredient.
                if !matched_allergens.contains(option) {
                    matching.insert(ing.to_owned(), option.to_owned());
                    matched_allergens.insert(option.clone());
                }
            }
        }
    }

    // finally, create the sorted list, and join the ingredients to create the string.
    let mut matching_vec = matching
        .iter()
        .map(|(k, v)| (k.to_owned(), v.to_owned()))
        .collect::<Vec<(String, String)>>();
    matching_vec.sort_by_key(|(_, v)| v.clone());

    let answer = matching_vec
        .into_iter()
        .map(|(k, _)| k)
        .collect::<Vec<String>>()
        .join(",");

    println!("Part two answer: {}", answer);

    Ok(())
}
