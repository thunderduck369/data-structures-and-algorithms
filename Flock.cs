using System.Collections.Generic;
using System.IO.IsolatedStorage;
using System.Text;
using AI.SteeringBehaviors.Core;

namespace AI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region TODO
        public Flock()
        {
            Boids = new List<MovingObject>();
            AverageForward = Vector3.Zero;
            AveragePosition = Vector3.Zero;
            AlignmentStrength = 1;
            CohesionStrength = 1;
            SeparationStrength = 1;
            FlockRadius = 10;
        }
        public virtual void Update(float deltaTime)
        {
            AverageForward = get_avg_forward();
            AveragePosition = get_avg_position();

            Vector3 accel = Vector3.Zero;
            foreach (MovingObject boid in Boids)
            {
                accel = calc_alignment_accel(boid);
                accel += calc_cohesion_accel(boid);
                accel += calc_separation_accel(boid);
                accel = accel * boid.MaxSpeed * deltaTime;
                boid.Velocity += accel;

                if (boid.Velocity.Length > boid.MaxSpeed)
                {
                    boid.Velocity.Normalize();
                    boid.Velocity = boid.Velocity * boid.MaxSpeed;
                }

                boid.Update(deltaTime);
            }
        }
        private Vector3 get_avg_forward()
        {
            Vector3 new_avg = Vector3.Zero;
            foreach (MovingObject boid in Boids)
            {
                new_avg += boid.Velocity;
            }
            new_avg /= Boids.Count;
            return new_avg;
        }
        private Vector3 get_avg_position()
        {
            Vector3 new_avg = Vector3.Zero;
            foreach (MovingObject boid in Boids)
            {
                new_avg += boid.Position;
            }
            new_avg /= Boids.Count;
            return new_avg;
        }
        private Vector3 calc_alignment_accel(MovingObject boid)
        {
            Vector3 accel = AverageForward / boid.MaxSpeed;

            if (accel.Length > 1)
            {
                accel.Normalize();
            }
            return accel * AlignmentStrength;
        }
        private Vector3 calc_cohesion_accel(MovingObject boid)
        {
            Vector3 accel = AveragePosition - boid.Position;
            float distance = accel.Length;
            accel.Normalize();

            if (distance < FlockRadius)
            {
                accel *= distance / FlockRadius;
            }
            return accel * CohesionStrength;
        }
        private Vector3 calc_separation_accel(MovingObject boid)
        {
            Vector3 sum = Vector3.Zero;

            foreach (MovingObject other in Boids)
            {
                if (other != boid)
                {
                    Vector3 accel = boid.Position - other.Position;
                    float distance = accel.Length;
                    float safeDistance = boid.SafeRadius + other.SafeRadius;
                    if (distance < safeDistance)
                    {
                        accel.Normalize();
                        accel = accel * (safeDistance - distance) / safeDistance;
                        sum += accel;
                    }
                }
            }

            if (sum.Length > 1)
            {
                sum.Normalize();
            }
            return sum * SeparationStrength;
        }
        #endregion
    }
}
