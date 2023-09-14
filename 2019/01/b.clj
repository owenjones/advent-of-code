(require '[clojure.string :as str])
(def masses (map read-string (str/split (slurp "input.txt") #"\n")))

(defn calculateFuelRequirements
  [mass]
  (let [fuel (int (- (Math/floor (/ mass 3)) 2))
        positiveFuel (if (> fuel 0) fuel 0) ; scrub fuel values < 0
        result (+ positiveFuel (if (> positiveFuel 0)
                                 (calculateFuelRequirements positiveFuel)
                                 0))]
    result))

(println (->> masses
              (map calculateFuelRequirements)
              (reduce +)))

; 4972784