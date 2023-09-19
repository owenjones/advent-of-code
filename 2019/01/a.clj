(require '[clojure.string :as str])
(def masses (map read-string (str/split-lines (slurp "input.txt"))))

(defn calculateFuelRequirements
  [mass]
  (int (- (Math/floor (/ mass 3)) 2)))

(println (->> masses
              (map calculateFuelRequirements)
              (reduce +)))

; 3317100