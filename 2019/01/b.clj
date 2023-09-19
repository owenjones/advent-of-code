(require '[clojure.string :as str])
(def masses (map read-string (str/split-lines (slurp "input.txt"))))

(defn calculateFuelRequirements
  [mass]
  (let [fuel (max (int (- (Math/floor (/ mass 3)) 2)) 0)
        result (+ fuel (if (> fuel 0)
                         (calculateFuelRequirements fuel)
                         0))]
    result))

(println (->> masses
              (map calculateFuelRequirements)
              (reduce +)))

; 4972784