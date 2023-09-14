(require '[clojure.string :as str])
(def masses (map read-string (str/split (slurp "input.txt") #"\n")))

(defn calculateFuelRequirements
  [mass]
  (int (- (Math/floor (/ mass 3)) 2)))

(println (->> masses
              (map calculateFuelRequirements)
              (reduce +)))

; 3317100